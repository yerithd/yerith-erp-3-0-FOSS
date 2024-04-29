/*
 * yerith-erp-qcheckbox.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_WIDGETS_YERITH_QCHECKBOX_HPP_
#define SRC_WIDGETS_YERITH_QCHECKBOX_HPP_


#include "src/utils/yerith-erp-utils.hpp"


#include <QtWidgets/QCheckBox>


class YerithERPQCheckBox : public QCheckBox
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS

	inline YerithERPQCheckBox(QWidget *parent)
	:QCheckBox(parent)
	{
	}

    virtual inline ~YerithERPQCheckBox()
    {
    }

    void setReadOnly(bool isReadOnly);

    void setYerithChecked(bool checked,
    					  const QString &text = YerithUtils::EMPTY_STRING);

    void setYerithEnabled(bool isEnabled);
};

#endif /* SRC_WIDGETS_YERITH_QCHECKBOX_HPP_ */
