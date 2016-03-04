// ======================================================================
// IMPROC: Image Processing Software Package
// Copyright (C) 2016 by George Wolberg
//
// Contrast.cpp - Brightness/Contrast widget.
//
// Written by: George Wolberg, 2016
//
// Modified by : Johannes Christ, 2016
// ======================================================================

#include "MainWindow.h"
#include "Contrast.h"
#include <iostream>

extern MainWindow *g_mainWindowP;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Contrast::Contrast:
//
// Constructor.
//
Contrast::Contrast(QWidget *parent) : ImageFilter(parent)
{}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Contrast::applyFilter:
//
// Run filter on the image, transforming I1 to I2.
// Overrides ImageFilter::applyFilter().
// Return 1 for success, 0 for failure.
//
bool
Contrast::applyFilter(ImagePtr I1, ImagePtr I2)
{
	// error checking
	if (I1.isNull()) return 0;

	// get contrast value
	double cont = m_sliderC->value() / 1.0;

	// get brightness value
	double bright = m_sliderB->value() / 1.0;

		
    // apply filter
	// brightness, contrast parameters
    contrast(I1, bright, cont, I2);

	return 1;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Contrast::createGroupBox:
//
// Create group box for control panel.
//
QGroupBox*
Contrast::controlPanel()
{
	// init group box
	m_ctrlGrp = new QGroupBox("Contrast");

	// init widgets
	// create label[i]
	QLabel *labelB = new QLabel;
	labelB->setText(QString("Brightnes"));

	QLabel *labelC = new QLabel;
	labelC->setText(QString("Contrast"));

	// create slider for Contrast
	m_sliderC = new QSlider(Qt::Horizontal, m_ctrlGrp);
	m_sliderC->setTickPosition(QSlider::TicksBelow);
	m_sliderC->setTickInterval(25);
	m_sliderC->setMinimum(-255);
	m_sliderC->setMaximum(255);
	m_sliderC->setValue(0);

	// create spinbox for Contrast
	m_spinBoxC = new QSpinBox(m_ctrlGrp);
	m_spinBoxC->setMinimum(-255);
	m_spinBoxC->setMaximum(255);
	m_spinBoxC->setValue(0);

	// create slider for Brightness
	m_sliderB = new QSlider(Qt::Horizontal, m_ctrlGrp);
	m_sliderB->setTickPosition(QSlider::TicksBelow);
	m_sliderB->setTickInterval(25);
	m_sliderB->setMinimum(-255);
	m_sliderB->setMaximum(255);
	m_sliderB->setValue(0);

	// create spinbox for Brightness
	m_spinBoxB = new QSpinBox(m_ctrlGrp);
	m_spinBoxB->setMinimum(-255);
	m_spinBoxB->setMaximum(255);
	m_spinBoxB->setValue(0);

	// init signal/slot connections for Threshold
	connect(m_sliderC, SIGNAL(valueChanged(int)), this, SLOT(changeCont(int)));
	connect(m_spinBoxC, SIGNAL(valueChanged(int)), this, SLOT(changeCont(int)));
	connect(m_sliderB, SIGNAL(valueChanged(int)), this, SLOT(changeBright(int)));
	connect(m_spinBoxB, SIGNAL(valueChanged(int)), this, SLOT(changeBright(int)));

	// assemble dialog
	QGridLayout *layout = new QGridLayout;
	layout->addWidget(labelB, 0, 0);
	layout->addWidget(m_sliderB, 1, 0);
	layout->addWidget(m_spinBoxB, 1, 1);
	layout->addWidget(labelC, 2, 0);
	layout->addWidget(m_sliderC, 3, 0);
	layout->addWidget(m_spinBoxC, 3, 1);


	// assign layout to group box
	m_ctrlGrp->setLayout(layout);

	return(m_ctrlGrp);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Contrast::changeCont:
//
// Slot to process change in thr caused by moving the slider.
//
void
Contrast::changeCont(int cont)
{
	m_sliderC->blockSignals(true);
	m_sliderC->setValue(cont);
	m_sliderC->blockSignals(false);
	m_spinBoxC->blockSignals(true);
	m_spinBoxC->setValue(cont);
	m_spinBoxC->blockSignals(false);

	// apply filter to source image; save result in destination image
	applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());

	// display output
	g_mainWindowP->displayOut();
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Contrast::changeBright:
//
// Slot to process change in thr caused by moving the slider.
//
void
Contrast::changeBright(int bright)
{
	m_sliderB->blockSignals(true);
	m_sliderB->setValue(bright);
	m_sliderB->blockSignals(false);
	m_spinBoxB->blockSignals(true);
	m_spinBoxB->setValue(bright);
	m_spinBoxB->blockSignals(false);

	// apply filter to source image; save result in destination image
	applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());

	// display output
	g_mainWindowP->displayOut();
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// contrast:
//
// 
//
void
Contrast::contrast(ImagePtr I1, double brightness, double contrast, ImagePtr I2)
{

	IP_copyImageHeader(I1, I2);
	int w = I1->width();
	int h = I1->height();
	int total = w * h;

	double factor = (259 * (contrast + 255)) / (255 * (259 - contrast));

	int type;
	ChannelPtr<uchar> p1, p2, endd;
	for (int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {

		IP_getChannel(I2, ch, p2, type);
		for (endd = p1 + total; p1 < endd;) {
			if ( ((int(factor * (*p1 - 128) + 128)) + brightness) > 255){
				*p2++ = 255;				
				*p1++;
			}

			else if ( ((int(factor * (*p1 - 128) + 128)) + brightness) < 0){
				*p2++ = 0;
				*p1++;
			}
			
			else
			*p2++ = int(factor * (*p1++ - 128) + 128) + brightness;		

		}

	}





}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Contrast::reset:
//
// Reset parameters.
//
void
Contrast::reset() {}
