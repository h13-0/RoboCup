from rknnlite.api import RKNNLite as RKNN
import cv2 as cv
import numpy as np


class YoloDetector():
    def __init__(self, ModulePath : str, Labels : list, InputWidth : int, InputHeight : int, Anchors : list, Mask : list,
        RowGrids : int = 0, ColGrids : int = 0,
        MaxBoxes : int = 500, ObjThresh : float = 0.5, NmsThresh : float = 0.1) -> None:
        '''
        @brief: Init YoloDetector.
        @param:
            - ModulePath:  Path of your yolo weight file(*.rknn).
            - Label:       Just label, such as: ["People", "Bird"].
            - InputWidth:  The width of the input RGB image.
            - InputHeight: The height of the input RGB image.
            - Anchors:     The anchors defined in the cfg file of Darknet.
            - Mask:        The mask defined in the cfg file of Darknet.
            - RowGrids:    The number of grid cells in a row, usually is the InputWidth / 32.
            - ColGrids:    The number of grid cells in a col, usually is the InputHeight / 32.
        
        @Default parameter reference:
            YOLO V3:
                Anchors = [[10, 13], [16, 30], [33, 23], [30, 61], [62, 45], [59, 119], [116, 90], [156, 198], [373, 326]]
                Masks = [[6, 7, 8], [3, 4, 5], [0, 1, 2]]
                
            YOLO V3 Tiny:
                Anchors = [[10, 14], [23, 27], [37, 58], [81, 82], [135, 169], [344, 319]]
                Masks = [[3, 4, 5], [0, 1, 2]]

            YOLO V4:
                Anchors = [[12, 16], [19, 36], [40, 28], [36, 75], [76, 55], [72, 146], [142, 110], [192, 243], [459, 401]]
                Masks = [[6, 7, 8], [3, 4, 5], [0, 1, 2]]
    
            YOLO V4 Tiny:
                Anchors = [[10, 14], [23, 27], [37, 58], [81, 82], [135, 169], [344, 319]]
                Masks = [[1, 2, 3], [3, 4, 5]]
        '''
        # Init Parameters.
        self.__labels__    = Labels
        self.__width__     = InputWidth
        self.__height__    = InputHeight
        self.__anchors__   = Anchors
        self.__mask__      = Mask
        self.__maxBox__    = MaxBoxes
        self.__objThresh__ = ObjThresh
        self.__nmsThresh__ = NmsThresh

        if(RowGrids == 0):
            self.__rowGrids__ = int(InputWidth / 32)
        
        if(ColGrids == 0):
            self.__colGrids__ = int(InputHeight / 32)
        
        self.__classesNumber__ = len(self.__labels__)
        self.__listSize__      = self.__classesNumber__ + 5
        self.__lgObjThresh__   = -np.log(1/ObjThresh - 1)

        # Create RKNN object
        self.__rknn__ = RKNN()

        # Load rknn model.
        ret = self.__rknn__.load_rknn(ModulePath)

        if ret != 0:
            raise Exception("Load rknn module failed with ret: " + ret)

        # Init rknn runtime.
        ret = self.__rknn__.init_runtime(target='rk1808', target_sub_class='AICS')

        if ret != 0:
            raise Exception("Init rknn runtime failed with ret: " + ret)

    
    def __sigmoid__(self, x):
        return 1 / (1 + np.exp(-x))

    
    def __calculateBox__(self, input, mask):
        anchors = [self.__anchors__[i] for i in mask]
        grid_h, grid_w = map(int, input.shape[0:2])

        box_confidence = input[..., 4]
        pos = np.where(box_confidence > self.__lgObjThresh__)
        input = input[pos]
        box_confidence = self.__sigmoid__(input[..., 4])
        box_confidence = np.expand_dims(box_confidence, axis=-1)

        box_class_probs = self.__sigmoid__(input[..., 5:])

        box_xy = self.__sigmoid__(input[..., :2])
        box_wh = np.exp(input[..., 2:4])
        for idx, val in enumerate(pos[2]):
            box_wh[idx] = box_wh[idx] * anchors[pos[2][idx]]
        pos0 = np.array(pos[0])[:, np.newaxis]
        pos1 = np.array(pos[1])[:, np.newaxis]
        grid = np.concatenate((pos1, pos0), axis=1)
        box_xy += grid
        box_xy /= (grid_w, grid_h)
        box_wh /= (960, 672)
        box_xy -= (box_wh / 2.)
        box = np.concatenate((box_xy, box_wh), axis=-1)

        return box, box_confidence, box_class_probs

    
    def __filter_boxes__(self, boxes, box_confidences, box_class_probs):
        """
        @brief: Filter boxes with object threshold.

        @param:
            boxes: ndarray, boxes of objects.
            box_confidences: ndarray, confidences of objects.
            box_class_probs: ndarray, class_probs of objects.

        @return:
            boxes: ndarray, filtered boxes.
            classes: ndarray, classes for boxes.
            scores: ndarray, scores for boxes.
        """
        box_scores = box_confidences * box_class_probs
        box_classes = np.argmax(box_scores, axis=-1)
        box_class_scores = np.max(box_scores, axis=-1)
        pos = np.where(box_class_scores >= self.__objThresh__)

        boxes = boxes[pos]
        classes = box_classes[pos]
        scores = box_class_scores[pos]

        return boxes, classes, scores

    
    def __nms_boxes__(self, boxes, scores):
        """Suppress non-maximal boxes.

        # Arguments
            boxes: ndarray, boxes of objects.
            scores: ndarray, scores of objects.

        # Returns
            keep: ndarray, index of effective boxes.
        """
        x = boxes[:, 0]
        y = boxes[:, 1]
        w = boxes[:, 2]
        h = boxes[:, 3]

        areas = w * h
        order = scores.argsort()[::-1]

        keep = []
        while order.size > 0:
            i = order[0]
            keep.append(i)

            xx1 = np.maximum(x[i], x[order[1:]])
            yy1 = np.maximum(y[i], y[order[1:]])
            xx2 = np.minimum(x[i] + w[i], x[order[1:]] + w[order[1:]])
            yy2 = np.minimum(y[i] + h[i], y[order[1:]] + h[order[1:]])

            w1 = np.maximum(0.0, xx2 - xx1 + 0.00001)
            h1 = np.maximum(0.0, yy2 - yy1 + 0.00001)
            inter = w1 * h1

            ovr = inter / (areas[i] + areas[order[1:]] - inter)
            inds = np.where(ovr <= self.__nmsThresh__)[0]
            order = order[inds + 1]
        keep = np.array(keep)
        return keep

    def __yoloPostProcess__(self, input_data, width, height):
        boxes, classes, scores = [], [], []
        for input, mask in zip(input_data, self.__mask__):
            b, c, s = self.__calculateBox__(input, mask)
            b, c, s = self.__filter_boxes__(b, c, s)
            boxes.append(b)
            classes.append(c)
            scores.append(s)

        boxes = np.concatenate(boxes)
        classes = np.concatenate(classes)
        scores = np.concatenate(scores)

        nboxes, nclasses, nscores = [], [], []
        for c in set(classes):
            inds = np.where(classes == c)
            b = boxes[inds]
            c = classes[inds]
            s = scores[inds]

            keep = self.__nms_boxes__(b, s)

            nboxes.append(b[keep])
            nclasses.append(c[keep])
            nscores.append(s[keep])

        if not nclasses and not nscores:
            return None, None, None

        boxes = np.concatenate(nboxes)
        classes = np.concatenate(nclasses)
        scores = np.concatenate(nscores)

        return boxes, classes, scores



    def Predict(self, BGR_Image) -> list:
        '''

        '''
        image = cv.resize(BGR_Image, (self.__width__, self.__height__))
        image = cv.cvtColor(image, cv.COLOR_BGR2RGB)
        
        out_boxes, out_boxes2 = self.__rknn__.inference(inputs=[image])

        out_boxes = out_boxes.reshape(3, self.__listSize__, self.__colGrids__, self.__rowGrids__)
        out_boxes2 = out_boxes2.reshape(3, self.__listSize__, self.__colGrids__ * 2, self.__rowGrids__ * 2)

        input_data = []
        input_data.append(np.transpose(out_boxes, (2, 3, 0, 1)))
        input_data.append(np.transpose(out_boxes2, (2, 3, 0, 1)))

        boxes, classes, scores = self.__yoloPostProcess__(input_data, self.__width__, self.__height__)
        
        if(boxes is not None):
            resultBoxes = []
            for box in boxes:
                x, y, w, h = box

                x *= BGR_Image.shape[1]
                y *= BGR_Image.shape[0]
                w *= BGR_Image.shape[1]
                h *= BGR_Image.shape[0]
                
                left = max(0, np.floor(x + 0.5).astype(int))
                top = max(0, np.floor(y + 0.5).astype(int))
                right = min(BGR_Image.shape[1], np.floor(x + w + 0.5).astype(int))
                bottom = min(BGR_Image.shape[0], np.floor(y + h + 0.5).astype(int))
                resultBoxes.append([left, top, right, bottom])

            return np.array(resultBoxes), classes, scores
        else:
            return boxes, classes, scores

    def __del__(self):
        self.__rknn__.release()


def LoadYoloV4_Tiny(ModulePath : str, Label : list, InputWidth : int, InputHeight : int) -> YoloDetector:
    '''
    @brief: Load Yolo V4 Tiny use default parameters.
    '''
    return YoloDetector(ModulePath, Label, InputWidth, InputHeight, 
        Anchors = [[10, 14], [23, 27], [37, 58], [81, 82], [135, 169], [344, 319]], 
        Mask = [[1, 2, 3], [3, 4, 5]])
