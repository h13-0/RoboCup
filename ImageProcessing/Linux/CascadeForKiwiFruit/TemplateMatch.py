import sys
import cv2
use_mask = False
img = None
templ = None
mask = None
image_window = "Source Image"
result_window = "Result window"
match_method = 0
max_Trackbar = 5
def main(argv):
    if (len(sys.argv) < 3):
        print('Not enough parameters')
        print('Usage:\nmatch_template_demo.py <image_name> <template_name> [<mask_name>]')
        return -1
    
    global img
    global templ
    img = cv2.imread(sys.argv[1], cv2.IMREAD_COLOR)
    templ = cv2.imread(sys.argv[2], cv2.IMREAD_COLOR)
    if (len(sys.argv) > 3):
        global use_mask
        use_mask = True
        global mask
        mask = cv2.imread( sys.argv[3], cv2.IMREAD_COLOR )
    if ((img is None) or (templ is None) or (use_mask and (mask is None))):
        print('Can\'t read one of the images')
        return -1
    
    
    cv2.namedWindow( image_window, cv2.WINDOW_AUTOSIZE )
    cv2.namedWindow( result_window, cv2.WINDOW_AUTOSIZE )
    
    
    trackbar_label = 'Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED'
    cv2.createTrackbar( trackbar_label, image_window, match_method, max_Trackbar, MatchingMethod )
    
    MatchingMethod(match_method)
    
    cv2.waitKey(0)
    return 0
    
def MatchingMethod(param):
    global match_method
    match_method = param
    
    img_display = img.copy()
    
    method_accepts_mask = (cv2.TM_SQDIFF == match_method or match_method == cv2.TM_CCORR_NORMED)
    if (use_mask and method_accepts_mask):
        result = cv2.matchTemplate(img, templ, match_method, None, mask)
    else:
        result = cv2.matchTemplate(img, templ, match_method)
    
    
    cv2.normalize( result, result, 0, 1, cv2.NORM_MINMAX, -1 )
    
    _minVal, _maxVal, minLoc, maxLoc = cv2.minMaxLoc(result, None)
    
    
    if (match_method == cv2.TM_SQDIFF or match_method == cv2.TM_SQDIFF_NORMED):
        matchLoc = minLoc
    else:
        matchLoc = maxLoc
    
    
    cv2.rectangle(img_display, matchLoc, (matchLoc[0] + templ.shape[0], matchLoc[1] + templ.shape[1]), (0,0,0), 2, 8, 0 )
    cv2.rectangle(result, matchLoc, (matchLoc[0] + templ.shape[0], matchLoc[1] + templ.shape[1]), (0,0,0), 2, 8, 0 )
    cv2.imshow(image_window, img_display)
    cv2.imshow(result_window, result)
    
    pass
if __name__ == "__main__":
    main(sys.argv[1:])