#include <qregexp.h>
#include <qstringlist.h> 
#include <math.h>

#include "TeQtLongEdit.h"

TeQtLongEdit::TeQtLongEdit(QWidget * parent, const char *name)
	:QLineEdit(parent, name)
{}

bool
TeQtLongEdit::decode(QString lado, short& dg, short& mn, float& sc, double& grauDec)
{
	short  	posat,lim = 180;

	double	dd,pi;			
	posat = 0;
	pi = 4.*atan((double)1.);

	// Longitude 
	if(dg < 0 || dg > lim)	
		return false;

	if(mn < 0 || mn > 60)
		return false;

	if(sc < 0. || sc > 60.)
		return false;

	if(lado == "W" || lado == "w"|| lado == "O"|| lado == "o")
	{
		if(dg > 0) dg = dg * -1;
		else if(mn > 0) mn = mn * -1;
		     else sc = sc * -1.;
	}

	dd = (double)(abs(dg)) + ((double)abs(mn)/60.) + fabs(sc)/3600.;
	if (dg < 0 || mn < 0 || (int)sc < 0)
		dd = -dd;

//	dd = dd*pi/180.;
	grauDec = dd;
	return true;
}


void TeQtLongEdit::focusOutEvent( QFocusEvent *e )
{
	int mat;
    QString str = text();
	QStringList myList;
	QRegExp exp1("^[oOwWeElL][ ]\\d\\d?\\d?[ ]?\\d\\d?[ ]?\\d?\\d?\\.?[0-9]?" );
	QRegExp exp2("^[oOwWeElL][;]\\d\\d?\\d?[;]?\\d\\d?[;]?\\d?\\d?\\.?[0-9]?" ); 
	QRegExp exp3("^[oOwWeElL][:]\\d\\d?\\d?[:]?\\d\\d?[:]?\\d?\\d?\\.?[0-9]?" ); 
	QRegExp exp4("^[+-]?\\d\\d?\\d?\\.?[0-9]?" ); 

	double grauDec;
	QString echo_;

    if ( (mat=exp1.match( str )) != -1 )
	{
		myList = QStringList::split(" ",str);
	}
	else if ((mat=exp2.match( str )) != -1 )
	{
		myList = QStringList::split(";",str);
	}
	else if ((mat=exp3.match( str )) != -1 )
	{
		myList = QStringList::split(":",str);
	}
	else if ((mat=exp4.match( str )) != -1 )
	{
		grauDec = str.toDouble();
		if ((grauDec >= -180.0 && grauDec <=180.0))
		{
			echo_.sprintf("%3.10f",grauDec);
			setText(echo_);
			emit coordValidator(grauDec);
			return;
		}
		else
		{
			setSelection( 0, text().length() );
			emit coordValidator(TeLONGINVALID);
			return;
		}
	}
	else
	{
		setSelection( 0, text().length() );
		emit coordValidator(TeLONGINVALID);
		return;
	}
	int nelementos = myList.count();
	short graus, minutos = 0;
	float segundos = 0.0;
	
	graus = myList[1].toShort();

	if (nelementos > 2)
		minutos = myList[2].toShort();
	
	if (nelementos > 3)
		segundos = myList[3].toFloat();

	bool dec = decode(myList[0],graus, minutos, segundos, grauDec);
	if (dec)
	{
		echo_.sprintf("%s %d %d %2.2f",(const char*) myList[0], abs(graus), minutos, segundos);
		setText(echo_);
		emit coordValidator(grauDec);
		return;
	}
	else
	{
		setSelection( 0, text().length() );
		emit coordValidator(TeLONGINVALID);
		return;
	}
}

void TeQtLongEdit::keyPressEvent( QKeyEvent *e )
{
    QLineEdit::keyPressEvent( e );
    if ( e->key() == Key_Enter || e->key() == Key_Return )
    {
        emit focusOutEvent(0);
    }
}