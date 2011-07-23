/*
 * Core AR
 * CRChainCodeBlob.cpp
 *
 * Copyright (c) Yuichi YOSHIDA, 11/07/23.
 * All rights reserved.
 * 
 * BSD License
 *
 * Redistribution and use in source and binary forms, with or without modification, are 
 * permitted provided that the following conditions are met:
 * - Redistributions of source code must retain the above copyright notice, this list of
 *  conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice, this list
 *  of conditions and the following disclaimer in the documentation and/or other materia
 * ls provided with the distribution.
 * - Neither the name of the "Yuichi Yoshida" nor the names of its contributors may be u
 * sed to endorse or promote products derived from this software without specific prior 
 * written permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY E
 * XPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES O
 * F MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SH
 * ALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENT
 * AL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROC
 * UREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS I
 * NTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRI
 * CT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF T
 * HE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "CRChainCodeBlob.h"

#include <math.h>

CRChainCodeBlob::CRChainCodeBlob() {
	_DPRINTF("CRChainCodeBlob constructor\n");
}

CRChainCodeBlob::~CRChainCodeBlob() {
	_DPRINTF("CRChainCodeBlob destructor\n");
	
	std::list<CRChainCodeElement*>::iterator it = elements.begin();
	while(it != elements.end()) {
		delete(*it);
		++it;
	}
}

void CRChainCodeBlob::appendChainCodeElement(int x, int y, int code) {
	CRChainCodeElement *element = new CRChainCodeElement(x, y, code);
	this->elements.push_back(element);
}

void CRChainCodeBlob::detectCorner() {
	CRChainCodeElement *firstCorner = this->firstCorner();
	CRChainCodeElement *thirdCorner = this->thirdCorner(firstCorner);
	CRChainCodeElement *secondCorner = this->secondCorner(firstCorner, thirdCorner);
	CRChainCodeElement *fourthCorner = this->fourthCorner(firstCorner, thirdCorner);
	
	_DPRINTF("%d,%d\n", firstCorner->x, firstCorner->y);
	_DPRINTF("%d,%d\n", secondCorner->x, secondCorner->y);
	_DPRINTF("%d,%d\n", thirdCorner->x, thirdCorner->y);
	_DPRINTF("%d,%d\n", fourthCorner->x, fourthCorner->y);
}

CRChainCodeElement* CRChainCodeBlob::firstCorner() {
	return this->elements.front();
}

CRChainCodeElement* CRChainCodeBlob::secondCorner(CRChainCodeElement *first, CRChainCodeElement *third) {
	float a = - (float)(third->y - first->y) / (third->x - first->x);
	float b = 1;
	float c = - a * first->x - first->y;
	float maxLength = 0;
	
	CRChainCodeElement* tempSecondCorner = NULL;
	std::list<CRChainCodeElement*>::iterator it = elements.begin();
	++it;
	while(it != elements.end()) {
		CRChainCodeElement* e = (CRChainCodeElement*)*it;
		
		if (e == third)
			break;
		
		float temp = fabs(a * e->x + b * e->y + c);
		
		if (temp > maxLength) {
			maxLength = temp;
			tempSecondCorner = e;
		}
		++it;
	}
	return tempSecondCorner;
}

CRChainCodeElement* CRChainCodeBlob::fourthCorner(CRChainCodeElement *first, CRChainCodeElement *third) {
	float a = - (float)(third->y - first->y) / (third->x - first->x);
	float b = 1;
	float c = - a * first->x - first->y;
	float maxLength = 0;
	
	CRChainCodeElement* tempThirdCorner = NULL;
	std::list<CRChainCodeElement*>::iterator it = elements.begin();
	while(it != elements.end()) {
		CRChainCodeElement* e = (CRChainCodeElement*)*it;
		if (e == third)
			break;
		++it;
	}
	while(it != elements.end()) {
		CRChainCodeElement* e = (CRChainCodeElement*)*it;
		
		float temp = fabs(a * e->x + b * e->y + c);
		
		if (temp > maxLength) {
			maxLength = temp;
			tempThirdCorner = e;
		}
		++it;
	}
	return tempThirdCorner;
}

CRChainCodeElement* CRChainCodeBlob::thirdCorner(CRChainCodeElement *first) {
	int maxLength = 0;
	CRChainCodeElement* tempThirdCorner = NULL;
	std::list<CRChainCodeElement*>::iterator it = elements.begin();
	while(it != elements.end()) {
		CRChainCodeElement* e = (CRChainCodeElement*)*it;
		int length = (first->x - e->x) * (first->x - e->x) + (first->y - e->y) * (first->y - e->y);
		if (length > maxLength) {
			maxLength = length;
			tempThirdCorner = e;
		}
		++it;
	}
	return tempThirdCorner;
}
