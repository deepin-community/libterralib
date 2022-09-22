#ifndef TEQTLATEDIT_H
#define TEQTLATEDIT_H

#include <qlineedit.h>
#include <qstring.h> 
#include <qwidget.h>

#define TeLATINVALID -99.99

class TeQtLatEdit : public QLineEdit
{
    Q_OBJECT
public:
	TeQtLatEdit(QWidget * parent, const char* name);
	bool decode(QString lado, short& dg, short& mn, float& sc, double& grauDec);

protected:
    void focusOutEvent( QFocusEvent *e );
    void keyPressEvent( QKeyEvent *e );

signals:
    void coordValidator(const double&);
};
#endif