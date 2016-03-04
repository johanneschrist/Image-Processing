// ======================================================================
// IMPROC: Image Processing Software Package
// Copyright (C) 2016 by Johannes Christ based on Prof. George Wolberg
//
// Quantization.h - Quantization widget
//
// Modified by: Johannes Christ, 2016
// ======================================================================

#ifndef QUANTIZATION_H
#define QUANTIZATION_H

#include "ImageFilter.h"


class Quantization : public ImageFilter {
	Q_OBJECT

public:
	Quantization(QWidget *parent = 0);		// constructor
	QGroupBox*	controlPanel();		// create control panel
	bool		applyFilter(ImagePtr, ImagePtr);// apply filter to input to init output
	void		reset();		// reset parameters

protected:
	void quantization(ImagePtr I1, int thr, ImagePtr I2);

	protected slots:
	void changeQuanta(int);

private:
	// quantization controls
	QSlider		*m_slider;
	QSpinBox	*m_spinBox;

	
	QLabel		*m_label;

	// widgets and groupbox
	QGroupBox	*m_ctrlGrp;	// Groupbox for panel
};


#endif	// QUANTIZATION_H
