#ifndef _YERITH_ERP_3_0_DEFINITION_SPECIAL_QT5_7_MACRO_HPP_
#define _YERITH_ERP_3_0_DEFINITION_SPECIAL_QT5_7_MACRO_HPP_

#define YERITH_QT_CONNECT_SIGNAL_TRIGGERED(QT_ACTION, QT_SLOT) \
    connect(QT_ACTION, SIGNAL(triggered()), this, SLOT(QT_SLOT));

#endif //_YERITH_ERP_3_0_DEFINITION_SPECIAL_QT5_7_MACRO_HPP_
