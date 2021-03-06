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
#include "Gamma.h"
#include <iostream>
#include <math.h>

extern MainWindow *g_mainWindowP;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 
// Constructor.
//
Gamma::Gamma(QWidget *parent) : ImageFilter(parent)
{}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// //
// Run filter on the image, transforming I1 to I2.
// Overrides ImageFilter::applyFilter().
// Return 1 for success, 0 for failure.
//
bool
Gamma::applyFilter(ImagePtr I1, ImagePtr I2)
{
	// error checking
	if (I1.isNull()) return 0;

	// get threshold value
	double thr = m_slider->value();

	// error checking
	//if (thr < 0 || thr > MXGRAY) return 0;

	// apply filter
	gamma(I1, thr, I2);

	return 1;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 
// Create group box for control panel.
//
QGroupBox*
Gamma::controlPanel()
{
	// init group box
	m_ctrlGrp = new QGroupBox("Gamma Correction");

	// init widgets
	// create label[i]
	QLabel *label = new QLabel;
	label->setText(QString("Gamma"));

	// create slider
	m_slider = new QSlider(Qt::Horizontal, m_ctrlGrp);
	m_slider->setTickPosition(QSlider::TicksBelow);
	m_slider->setTickInterval(50);
	m_slider->setMinimum(0.01);
	m_slider->setMaximum(7.99);
	m_slider->setValue(4);

	// create spinbox
	m_spinBox = new QSpinBox(m_ctrlGrp);
	m_spinBox->setMinimum(0.01);
	m_spinBox->setMaximum(7.99);
	m_spinBox->setValue(4);

	// init signal/slot connections for Threshold
	connect(m_slider, SIGNAL(valueChanged(int)), this, SLOT(changeGamma(int)));
	connect(m_spinBox, SIGNAL(valueChanged(int)), this, SLOT(changeGamma(int)));

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
// Gamma::changeGamma:
//
// Slot to process change in thr caused by moving the slider.
//
void
Gamma::changeGamma(int thr)
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
Gamma::gamma(ImagePtr I1, double thr, ImagePtr I2) {
	IP_copyImageHeader(I1, I2);
	int w = I1->width();
	int h = I1->height();
	int total = w * h;

	//double factor = (259 * (thr + 255)) / (255 * (259 - contrast));

	int type;
	ChannelPtr<uchar> p1, p2, endd;
	for (int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {		

		double gammaCorrection = 1.0 / thr;
		std::cout << "\nGamma correction is: " << gammaCorrection;
		//newRed = 255 * (Red(colour) / 255) ^ gammaCorrection
		//	pow(7.0, 3.0)

		IP_getChannel(I2, ch, p2, type);
		for (endd = p1 + total; p1 < endd;) {

			if (  ( int( 255 * pow((*p1 / 255.0), (gammaCorrection)))) > 255  ){
				//std::cout << "\nColor is: " << (int(255 * pow((*p1 / 255), (gammaCorrection))));
				*p2++ = 255;
				*p1++;
			}

			else if ((int(255 * pow((*p1 / 255.0), (gammaCorrection)))) < 0){
				//std::cout << "\nColor is: " << (int(255 * pow((*p1 / 255), (gammaCorrection))));
				*p2++ = 0;
				*p1++;
			}

			else{
				//std::cout << "\nColor is: " << (int(255 * pow((*p1 / 255), (gammaCorrection))));
				*p2++ = (int(255 * pow((*p1++ / 255.0), (gammaCorrection))));
			}
			
		}

	}



}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Gamma::reset:
//
// Reset parameters.
//
void
Gamma::reset() {}
