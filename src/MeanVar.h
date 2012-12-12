/**
 *  \file		MeanVar.h
 *  \brief		Implements a class to calculate the mean value.
 *  \details	Used type is double
 *  \author		Sebastian Eibl
 *  \version	1.0
 *  \date		02/11/2012
 *  \pre		none
 *  \bug		none
 *  \warning	none
 *  \copyright	Sebastian Eibl
 */

#ifndef MEANVAR_HPP
#define MEANVAR_HPP

#include	<cmath>

class	MeanVar{
	private:
		double	m_x;
		double	m_x2;
		unsigned int m_n;
	public:
		MeanVar();

		void	Add(const double& x);
		void	Clear(const double& null);
		double	Mean(); //inline
		double	Var(); //inline
		double	Sigma(); //inline
		double	Error(); //inline

};

inline
void MeanVar::Add(const double& x){
	m_x += x;
	m_x2 += x*x;
	m_n++;
}

inline
void MeanVar::Clear(const double& null){
	m_x = null;
	m_x2 = null;
	m_n = 0;
}

inline
double	MeanVar::Mean(){
	return	m_x/m_n;
}

inline
double	MeanVar::Var(){
	return	m_x2/m_n - Mean()*Mean();
}

inline
double	MeanVar::Sigma(){
	return	sqrt(Var());
}

inline
double	MeanVar::Error(){
	return	Sigma()/sqrt(static_cast<double>(m_n));
}
#endif
