#pragma once

class FXmlNode;

// Represents a curve of integer values as function of level.
// Form is f(L) = K*L^T where L is the level, and K and T are
// constants.
class Curve
{
public:
	Curve();
	Curve( double scalar, double exponent );
	
	int GetValue( int level ) const;
	
	void FromXml( FXmlNode * const node );
	
private:
	double _scalar;
	double _exponent;
};
