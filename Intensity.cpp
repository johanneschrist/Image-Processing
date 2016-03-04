// ======================================================================
// IMPROC: Image Processing Software Package
// Copyright (C) 2016 by George Wolberg
//
// Threshold.cpp - Threshold class
//
// Written by: George Wolberg, 2016

// Modified by : Johannes Christ, 2016
// ======================================================================

#include "MainWindow.h"
#include "Intensity.h"

extern MainWindow *g_mainWindowP;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Constructor.
//
Intensity::Intensity(QWidget *parent) : ImageFilter(parent)
{}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Threshold::applyFilter:
//
// Run filter on the image, transforming I1 to I2.
// Overrides ImageFilter::applyFilter().
// Return 1 for success, 0 for failure.
//
bool
Intensity::applyFilter(ImagePtr I1, ImagePtr I2)
{
	// error checking
	if (I1.isNull()) return 0;

	// get threshold value
	int thr = m_slider->value();

	// error checking
	if (thr < 0 || thr > MXGRAY) return 0;

	// apply filter
	intensity(I1, thr, I2);

	return 1;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Threshold::controlPanel:
//
// Create group box for control panel.
//
QGroupBox*
Intensity::controlPanel()
{
	// init group box
	m_ctrlGrp = new QGroupBox("Intensity");

	// init widgets
	// create label[i]
	QLabel *label = new QLabel;
	label->setText(QString("Intensity"));

	// create slider
	m_slider = new QSlider(Qt::Horizontal, m_ctrlGrp);
	m_slider->setTickPosition(QSlider::TicksBelow);
	m_slider->setTickInterval(25);
	m_slider->setMinimum(1);
	m_slider->setMaximum(MXGRAY);
	m_slider->setValue(MXGRAY >> 1);

	// create spinbox
	m_spinBox = new QSpinBox(m_ctrlGrp);
	m_spinBox->setMinimum(1);
	m_spinBox->setMaximum(MXGRAY);
	m_spinBox->setValue(MXGRAY >> 1);

	// init signal/slot connections
	connect(m_slider, SIGNAL(valueChanged(int)), this, SLOT(changeInt(int)));
	connect(m_spinBox, SIGNAL(valueChanged(int)), this, SLOT(changeInt(int)));

	// assemble dialog
	QGridLayout *layout = new QGridLayout;
	layout->addWidget(label, 0, 0);
	layout->addWidget(m_slider, 0, 1);
	layout->addWidget(m_spinBox, 0, 2);

	// assign layout to group box
	m_ctrlGrp->setLayout(layout);

	return(m_ctrlGrp);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 
//
// Slot to process change in thr caused by moving the slider.
//
void
Intensity::changeInt(int thr)
{
	m_slider->blockSignals(true);
	m_slider->setValue(thr);
	m_slider->blockSignals(false);
	m_spinBox->blockSignals(true);
	m_spinBox->setValue(thr);
	m_spinBox->blockSignals(false);

	// apply filter to source image; save result in destination image
	applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());

	// display output
	g_mainWindowP->displayOut();
}




void
Intensity::intensity(ImagePtr I1, int thr, ImagePtr I2) {
	IP_copyImageHeader(I1, I2);
	int w = I1->width();
	int h = I1->height();
	int total = w * h;

	// compute lut[]
	int i, lut[MXGRAY];
	for (i = 0; i<thr && i<MXGRAY; ++i) lut[i] = 0;
	for (; i <= MaxGray; ++i) lut[i] = MaxGray;

	int type;
	ChannelPtr<uchar> p1, p2, endd;
	for (int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
		IP_getChannel(I2, ch, p2, type);
		for (endd = p1 + total; p1<endd;) *p2++ = lut[*p1++];
	}
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Intensity::reset:
//
// Reset parameters.
//
void
Intensity::reset() {}
