// ======================================================================
// IMPROC: Image Processing Software Package
// Copyright (C) 2016 by George Wolberg
//
// Threshold.h - Threshold widget
//
// Written by: George Wolberg, 2016

// Modified by : Johannes Christ, 2016
// ======================================================================

#ifndef INTENSITY_H
#define INTENSITY_H

#include "ImageFilter.h"


class Intensity : public ImageFilter {
	Q_OBJECT

public:
	Intensity(QWidget *parent = 0);		// constructor
	QGroupBox*	controlPanel();		// create control panel
	bool		applyFilter(ImagePtr, ImagePtr);// apply filter to input to init output
	void		reset();		// reset parameters

protected:
	void intensity(ImagePtr I1, int thr, ImagePtr I2);

	protected slots:
	void changeInt(int);

private:
	// threshold controls
	QSlider		*m_slider;	
	QSpinBox	*m_spinBox;	

	
	QLabel		*m_label;

	// widgets and groupbox
	QGroupBox	*m_ctrlGrp;	// Groupbox for panel
};


#endif	// INTENSITY_H
