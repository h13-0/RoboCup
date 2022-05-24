#include "HSV_RangeWindow.h"
#include "QString.h"

HSV_Range::HSV_RangeWindow::HSV_RangeWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	//Callback functions.
	SliderChangedCallback_t hMaxSliderChangedCallback =
		[this](unsigned int Value)
	{
		this->ui.H_MaxValue->setText(QString("%1").arg(Value));
	};

	SliderChangedCallback_t hMinSliderChangedCallback =
		[this](unsigned int Value)
	{
		this->ui.H_MinValue->setText(QString("%1").arg(Value));
	};

	SliderChangedCallback_t sMaxSliderChangedCallback =
		[this](unsigned int Value)
	{
		this->ui.S_MaxValue->setText(QString("%1").arg(Value));
	};

	SliderChangedCallback_t sMinSliderChangedCallback =
		[this](unsigned int Value)
	{
		this->ui.S_MinValue->setText(QString("%1").arg(Value));
	};

	SliderChangedCallback_t vMaxSliderChangedCallback =
		[this](unsigned int Value)
	{
		this->ui.V_MaxValue->setText(QString("%1").arg(Value));
	};

	SliderChangedCallback_t vMinSliderChangedCallback =
		[this](unsigned int Value)
	{
		this->ui.V_MinValue->setText(QString("%1").arg(Value));
	};

	ButtonClickedCallback_t resetThresholdsButtonCallback =
		[this](void)
	{
		this->ui.H_MinSlider->setValue(0);
		this->ui.H_MaxSlider->setValue(255);
		this->ui.S_MinSlider->setValue(0);
		this->ui.S_MaxSlider->setValue(255);
		this->ui.V_MinSlider->setValue(0);
		this->ui.V_MaxSlider->setValue(255);
	};
	
	//Add callback functions.
	AddHueMaximumSliderCallback(hMaxSliderChangedCallback);
	AddHueMinimumSliderCallback(hMinSliderChangedCallback);
	AddSaturationMaximumSliderCallback(sMaxSliderChangedCallback);
	AddSaturationMinimumSliderCallback(sMinSliderChangedCallback);
	AddValueMaximumSliderCallback(vMaxSliderChangedCallback);
	AddValueMinimumSliderCallback(vMinSliderChangedCallback);
	AddResetThresholdsButtonCallback(resetThresholdsButtonCallback);

	//Connecting signal slots.
	connect(this, &HSV_RangeWindow::RefreshImage, this, &HSV_RangeWindow::refreshImageMethod, Qt::BlockingQueuedConnection);
	connect(this, &HSV_RangeWindow::GetLineEditText, this, &HSV_RangeWindow::getLineEditTextMethod);
	connect(this, &HSV_RangeWindow::SetOpenVideoButtonText, this, [this](std::string Text) {this->ui.OpenVideoButton->setText(QString::fromStdString(Text)); });

	connect(this->ui.RGB_Preview, &QRadioButton::toggled, this, &HSV_RangeWindow::rgbPreviewRadioButtonChanged);
	connect(this->ui.HSV_Preview, &QRadioButton::toggled, this, &HSV_RangeWindow::hsvPreviewRadioButtonChanged);
	connect(this->ui.H_MaxSlider, &QSlider::valueChanged, this, &HSV_RangeWindow::hueMaximumSliderChanged);
	connect(this->ui.H_MinSlider, &QSlider::valueChanged, this, &HSV_RangeWindow::hueMinimumSliderChanged);
	connect(this->ui.S_MaxSlider, &QSlider::valueChanged, this, &HSV_RangeWindow::saturationMaximumSliderChanged);
	connect(this->ui.S_MinSlider, &QSlider::valueChanged, this, &HSV_RangeWindow::saturationMinimumSliderChanged);
	connect(this->ui.V_MaxSlider, &QSlider::valueChanged, this, &HSV_RangeWindow::valueMaximumSliderChanged);
	connect(this->ui.V_MinSlider, &QSlider::valueChanged, this, &HSV_RangeWindow::valueMinimumSliderChanged);
	connect(this->ui.OpenPhotoButton, &QPushButton::clicked, this, &HSV_RangeWindow::openPhotoButtonClicked);
	connect(this->ui.OpenVideoButton, &QPushButton::clicked, this, &HSV_RangeWindow::openVideoButtonClicked);
	connect(this->ui.ResetThresholdsButton, &QPushButton::clicked, this, &HSV_RangeWindow::resetThresholdsButtonClicked);
}

void HSV_Range::HSV_RangeWindow::refreshImageMethod(cv::Mat Image)
{
	using namespace cv;
	static QGraphicsScene scene;

	cvtColor(Image, Image, COLOR_BGR2RGB);

	QImage disImage = QImage((const unsigned char*)(Image.data), Image.cols, Image.rows, QImage::Format_RGB888);
	scene.clear();
	QGraphicsPixmapItem *pointer = scene.addPixmap(QPixmap::fromImage(disImage));

	ui.graphicsView->setScene(&scene);
	ui.graphicsView->fitInView(ui.graphicsView->scene()->sceneRect(), Qt::KeepAspectRatio);
	ui.graphicsView->show();
}

void HSV_Range::HSV_RangeWindow::addPreviewModeChangedCallbackFunction(PreviewModeChangedCallback_t& Callback, std::vector<PreviewModeChangedCallback_t>& TargetCallbacks)
{
	TargetCallbacks.push_back(Callback);
}

void HSV_Range::HSV_RangeWindow::addSliderChangedCallbackFunction(SliderChangedCallback_t& Callback, std::vector<SliderChangedCallback_t>& TargetCallbacks)
{
	TargetCallbacks.push_back(Callback);
}

void HSV_Range::HSV_RangeWindow::addButtonClickedCallbackFunction(ButtonClickedCallback_t& Callback, std::vector<ButtonClickedCallback_t>& TargetCallbacks)
{
	TargetCallbacks.push_back(Callback);
}

void HSV_Range::HSV_RangeWindow::executePreviewModeChangedCallback(const std::vector<PreviewModeChangedCallback_t>& Callbacks, PreviewMode_t NewMode)
{
	for (auto& callback : Callbacks)
	{
		callback(NewMode);
	}
}

void HSV_Range::HSV_RangeWindow::executeSliderChangedCallback(const std::vector<SliderChangedCallback_t>& Callbacks, unsigned int CurrentValue)
{
	for (auto& callback : Callbacks)
	{
		callback(CurrentValue);
	}
}

void HSV_Range::HSV_RangeWindow::executeButtonClickedCallback(const std::vector<ButtonClickedCallback_t>& Callbacks)
{
	for (auto& callback : Callbacks)
	{
		callback();
	}
}

std::string HSV_Range::HSV_RangeWindow::getLineEditTextMethod(QLineEdit* LineEdit)
{
	using namespace std;
	try {
		return LineEdit->text().toStdString();
	}
	catch (...)
	{
		return "";
	}
}
