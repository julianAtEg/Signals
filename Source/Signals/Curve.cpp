#include "Signals.h"
#include "Curve.h"
#include <cmath>
#include "XmlParser.h"

Curve::Curve()
: Curve( 1.0, 1.0 )
{
}

Curve::Curve( double scalar, double exponent )
: _scalar( scalar )
, _exponent( exponent )
{
}

int Curve::GetValue( int level ) const
{
	return( int(_scalar*pow( (double)level, _exponent )) );
}

void Curve::FromXml( FXmlNode * const node )
{
	auto scalarStr = node->GetAttribute(TEXT("scalar"));
	check(!scalarStr.IsEmpty());
	_scalar = FCString::Atof(*scalarStr);
	
	auto exponentStr = node->GetAttribute(TEXT("exponent"));
	check(!exponentStr.IsEmpty());
	_exponent = FCString::Atof(*exponentStr);
}