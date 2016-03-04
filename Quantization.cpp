// ======================================================================
// IMPROC: Image Processing Software Package
// Copyright (C) 2016 by George Wolberg
//
// Quantization.cpp - Threshold class
//
// Written by: George Wolberg, 2016

// Modified by : Johannes Christ, 2016
// ======================================================================

#include "MainWindow.h"
#include "Quantization.h"

extern MainWindow *g_mainWindowP;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Constructor.
//
Quantization::Quantization(QWidget *parent) : ImageFilter(parent)
{}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Run filter on the image, transforming I1 to I2.
// Overrides ImageFilter::applyFilter().
// Return 1 for success, 0 for failure.
//
bool
Quantization::applyFilter(ImagePtr I1, ImagePtr I2)
{
	// error checking
	if (I1.isNull()) return 0;

	// get value
	int levels = m_slider->value();

	
	// apply filter
	quantization(I1, levels, I2);

	return 1;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 
//
// Create group box for control panel.
//
QGroupBox*
Quantization::controlPanel()
{
	// init group box
	m_ctrlGrp = new QGroupBox("Quantization");

	// init widgets
	// create label[i]
	QLabel *label = new QLabel;
	label->setText(QString("Quant"));

	// create slider
	m_slider = new QSlider(Qt::Horizontal, m_ctrlGrp);
	m_slider->setTickPosition(QSlider::TicksBelow);
	m_slider->setTickInterval(25);
	m_slider->setMinimum(1);
	m_slider->setMaximum(15);
	m_slider->setValue(4);

	// create spinbox
	m_spinBox = new QSpinBox(m_ctrlGrp);
	m_spinBox->setMinimum(1);
	m_spinBox->setMaximum(15);
	m_spinBox->setValue(4);

	// init signal/slot connections
	connect(m_slider, SIGNAL(valueChanged(int)), this, SLOT(changeQuanta(int)));
	connect(m_spinBox, SIGNAL(valueChanged(int)), this, SLOT(changeQuanta(int)));

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
// Slot to process change in thr caused by moving the slider.
//
void
Quantization::changeQuanta(int levels)
{
	m_slider->blockSignals(true);
	m_slider->setValue(levels);
	m_slider->blockSignals(false);
	m_spinBox->blockSignals(true);
	m_spinBox->setValue(levels);
	m_spinBox->blockSignals(false);

	// apply filter to source image; save result in destination image
	applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());

	// display output
	g_mainWindowP->displayOut();
}




void
Quantization::quantization(ImagePtr I1, int levels, ImagePtr I2) {
	IP_copyImageHeader(I1, I2);
	int w = I1->width();
	int h = I1->height();
	int total = w * h;

	// compute lut[]
	int i, lut[MXGRAY];

	// init lookup tables
	int scale = MXGRAY / levels;
	for (i = 0; i<MXGRAY; i++)
		lut[i] = scale*(int)(i / scale);
	// iterate over all pixels
	//for (i = 0; i<total; i++) out[i] = lut[in[i]];
	//////////////

	int type;
	ChannelPtr<uchar> p1, p2, endd;
	for (int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
		IP_getChannel(I2, ch, p2, type);
		for (endd = p1 + total; p1<endd;) *p2++ = lut[*p1++];
	}
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Reset parameters.
//
void
Quantization::reset() {}
