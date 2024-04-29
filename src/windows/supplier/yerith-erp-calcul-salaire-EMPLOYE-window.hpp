/*
 * yerith-erp-calcul-salaire-EMPLOYE-window.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef YERITH_CALCUL_SALAIRE_EMPLOYE_WINDOW_HPP_
#define YERITH_CALCUL_SALAIRE_EMPLOYE_WINDOW_HPP_


#include "src/windows/yerith-erp-window-commons.hpp"

#include "src/utils/yerith-erp-payment-processing-information.hpp"


#include <QtWidgets/QMessageBox>



class YerithERPWindows;
class YerithSqlTableModel;


class YerithERPCalculSalaireEMPLOYE : public QObject
{
	Q_OBJECT


public:


    YERITH_CLASS_OPERATORS


    inline YerithERPCalculSalaireEMPLOYE()
    :_sommeTotal(0.0),
     _calling_window(0)
    {
        __QMESSAGE_BOX_STYLE_SHEET =
                        QString("QMessageBox {background-color: rgb(%1);}"
                                "QMessageBox QLabel {color: rgb(%2);}")
                            .arg(COLOUR_RGB_STRING_YERITH_GREEN_2_160_70,
                                 COLOUR_RGB_STRING_YERITH_WHITE_255_255_255);
    }


	YerithERPCalculSalaireEMPLOYE(YerithWindowsCommons *calling_window);


    inline ~YerithERPCalculSalaireEMPLOYE()
    {
    }


    bool INSERT_employee_Paiement_AS_PAYMENT_ENTRY_DB_TABLE
            (YerithERPPaymentProcessingInformation &Payment_processing_info_In);


    double calculate_PAY_GROUP_MONEY_BENEFITS(QString &a_nom_entreprise_EMPLOYEE,
                                              QString &PAY_GROUP,
                                              YerithERPPaymentProcessingInformation *payment_processing_info_IN_Out = 0);


    double CALCULATE_EMPLOYEE_SALARY(const QString &a_nom_entreprise_EMPLOYEE);


    double CALCULATE_EMPLOYEE_SALARY(YerithERPPaymentProcessingInformation  &payment_processing_info_IN_Out,
                                     const QString                          &a_nom_entreprise_EMPLOYEE);


private:

    double _sommeTotal;

    QString __QMESSAGE_BOX_STYLE_SHEET;

    YerithWindowsCommons *_calling_window;
};


#endif /* YERITH_CALCUL_SALAIRE_EMPLOYE_WINDOW_HPP_ */
