#ifndef TEQTLONGEDIT_H
#define TEQTLONGEDIT_H

#include <qlineedit.h>
#include <qstring.h>
#include <qwidget.h>

#define TeLONGINVALID -999.99

class TeQtLongEdit : public QLineEdit
{
    Q_OBJECT
public:
	TeQtLongEdit(QWidget *parent, const char* name);
	bool decode(QString lado, short& dg, short& mn, float& sc, double& grauDec);

protected:
    void focusOutEvent( QFocusEvent *e );
    void keyPressEvent( QKeyEvent *e );

signals:
    void coordValidator(const double&);
};
#endif