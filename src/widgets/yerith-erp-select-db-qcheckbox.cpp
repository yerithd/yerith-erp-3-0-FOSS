/*
 * yerith-erp-select-db-qcheckbox.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */


#include "yerith-erp-select-db-qcheckbox.hpp"


#include <QtCore/QDebug>


YerithSelectDBQCheckBox::YerithSelectDBQCheckBox(QWidget *parent, QStringList *visibleDBFieldColumnStrList)
:YerithERPQCheckBox(parent),
 _visibleDBFieldColumnStrList(visibleDBFieldColumnStrList)
{
}


void YerithSelectDBQCheckBox::handle_visible_db_field_checkBox(bool checkBoxClicked)
{
    if (0 != _visibleDBFieldColumnStrList)
    {
        if (checkBoxClicked)
        {
            if (!_visibleDBFieldColumnStrList->contains(objectName()))
            {
                _visibleDBFieldColumnStrList->append(objectName());
            }
        }
        else
        {
            _visibleDBFieldColumnStrList->removeAll(objectName());
        }
    }
}


