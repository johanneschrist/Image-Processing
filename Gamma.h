// ======================================================================
// IMPROC: Image Processing Software Package
// Copyright (C) 2016 by George Wolberg
//
// Threshold.h - Threshold widget
//
// Written by: George Wolberg, 2016

// Modified by : Johannes Christ, 2016
// ======================================================================

#ifndef GAMMA_H
#define GAMMA_H

#include "ImageFilter.h"


class Gamma : public ImageFilter {
	Q_OBJECT

public:
	Gamma(QWidget *parent = 0);		// constructor
	QGroupBox*	controlPanel();		// create control panel
	bool		applyFilter(ImagePtr, ImagePtr);// apply filter to input to init output
	void		reset();		// reset parameters

protected:
	void gamma(ImagePtr I1, int thr, ImagePtr I2);

	protected slots:
	void changeGamma(int);

private:
	// threshold controls
	QSlider		*m_slider;	// Contrast sliders
	QSpinBox	*m_spinBox;	// Contrast spin boxes

	QLabel		*m_label;	

	// widgets and groupbox
	QGroupBox	*m_ctrlGrp;	// Groupbox for panel
};


#endif	// GAMMA_H
