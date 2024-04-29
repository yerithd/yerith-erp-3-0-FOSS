/**
 * modifier-client.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

void YerithAdminModifierWindow::setupEditClient()
{
    _windowName = QString("%1 - %2")
    				.arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
                		 QObject::tr("administration ~ modifier ~ clients"));

    YerithAdminListerWindow *lw = _allWindows->_adminListerWindow;

    YerithSqlTableModel *clientsTableModel = lw->getCurSearchSqlTableModel();

    if (!clientsTableModel)
    {
        clientsTableModel = &_allWindows->getSqlTableModel_clients();
    }
    else if (clientsTableModel &&
             !YerithUtils::isEqualCaseInsensitive(clientsTableModel->
                                                  sqlTableName(),
                                                  YerithDatabase::CLIENTS))
    {
        clientsTableModel = &_allWindows->getSqlTableModel_clients();
    }

    QSqlRecord record =
                    clientsTableModel->record(lw->lastSelectedItemForModification());

    lineEdit_modifier_client_reference_client->setText(GET_SQL_RECORD_DATA
                                                       (record,
                                                        YerithDatabaseTableColumn::REFERENCE_CLIENT));
    lineEdit_modifier_client_nom_entreprise->setText(GET_SQL_RECORD_DATA
                                                     (record,
                                                      YerithDatabaseTableColumn::NOM_ENTREPRISE));
    lineEdit_modifier_client_nom_representant->setText(GET_SQL_RECORD_DATA
                                                       (record,
                                                        YerithDatabaseTableColumn::NOM_REPRESENTANT));
    lineEdit_modifier_client_quartier->setText(GET_SQL_RECORD_DATA
                                               (record,
                                                YerithDatabaseTableColumn::QUARTIER));
    lineEdit_modifier_client_ville->setText(GET_SQL_RECORD_DATA
                                            (record,
                                             YerithDatabaseTableColumn::VILLE));
    lineEdit_modifier_client_province_etat->setText(GET_SQL_RECORD_DATA
                                                    (record, "province_etat"));
    lineEdit_modifier_client_pays->setText(GET_SQL_RECORD_DATA
                                           (record,
                                            YerithDatabaseTableColumn::PAYS));
    lineEdit_modifier_client_boite_postale->setText(GET_SQL_RECORD_DATA
                                                    (record,
                                                     YerithDatabaseTableColumn::BOITE_POSTALE));
    lineEdit_modifier_client_siege_social->setText(GET_SQL_RECORD_DATA
                                                   (record,
                                                    YerithDatabaseTableColumn::SIEGE_SOCIAL));
    lineEdit_modifier_client_email->setText(GET_SQL_RECORD_DATA
                                            (record,
                                             YerithDatabaseTableColumn::EMAIL));
    lineEdit_modifier_client_numero_telephone_1->setText(GET_SQL_RECORD_DATA
                                                         (record,
                                                          YerithDatabaseTableColumn::NUMERO_TELEPHONE_1));
    lineEdit_modifier_client_numero_telephone_2->setText(GET_SQL_RECORD_DATA
                                                         (record,
                                                          YerithDatabaseTableColumn::NUMERO_TELEPHONE_2));
    lineEdit_modifier_client_numero_contribuable->setText(GET_SQL_RECORD_DATA
                                                          (record,
                                                           YerithDatabaseTableColumn::NUMERO_CONTRIBUABLE));

    if (0 != YerithUtils::getAllWindows())
    {
        YerithPOSUser *aUser = YerithUtils::getAllWindows()->getUser();

        if (0 != aUser && aUser->isManager())
        {
            label_admin_modifier_client_dette_maximale_compte_client->setVisible
            (true);
            lineEdit_modifier_client_dette_maximale_compte_client->setVisible
            (true);
            lineEdit_modifier_client_dette_maximale_compte_client->setEnabled
            (true);

            double dette_maximale_compte_client = GET_SQL_RECORD_DATA(record,
                                                                      YerithDatabaseTableColumn::
                                                                      DETTE_MAXIMALE_COMPTE_CLIENT).toDouble
                                                  ();

            lineEdit_modifier_client_dette_maximale_compte_client->setText
            (QString::number(dette_maximale_compte_client));
        }
        else
        {
            label_admin_modifier_client_dette_maximale_compte_client->setVisible
            (false);
            lineEdit_modifier_client_dette_maximale_compte_client->setVisible
            (false);
            lineEdit_modifier_client_dette_maximale_compte_client->setEnabled
            (false);
        }
    }
    else
    {
        label_admin_modifier_client_dette_maximale_compte_client->setVisible
        (false);
        lineEdit_modifier_client_dette_maximale_compte_client->setVisible
        (false);
        lineEdit_modifier_client_dette_maximale_compte_client->setEnabled
        (false);
    }

    textEdit_modifier_client_description->setText(GET_SQL_RECORD_DATA
                                                  (record,
                                                   YerithDatabaseTableColumn::DESCRIPTION_CLIENT));
}

void YerithAdminModifierWindow::modifier_client()
{
    //_logger->log("modifier_client");
    if (modifier_client_check_fields())
    {
        YerithAdminListerWindow *lw = _allWindows->_adminListerWindow;
        YerithSqlTableModel *clientsTableModel =
                        lw->getCurSearchSqlTableModel();

        if (!clientsTableModel)
        {
            clientsTableModel = &_allWindows->getSqlTableModel_clients();
        }
        else if (clientsTableModel &&
                 !YerithUtils::isEqualCaseInsensitive(clientsTableModel->
                                                      sqlTableName(),
                                                      YerithDatabase::CLIENTS))
        {
            clientsTableModel = &_allWindows->getSqlTableModel_clients();
        }

        QSqlRecord record =
                        clientsTableModel->record(lw->lastSelectedItemForModification());


        record.setValue(YerithDatabaseTableColumn::REFERENCE_CLIENT,
                        lineEdit_modifier_client_reference_client->text());
        record.setValue(YerithDatabaseTableColumn::NOM_ENTREPRISE,
                        lineEdit_modifier_client_nom_entreprise->text());
        record.setValue(YerithDatabaseTableColumn::NOM_REPRESENTANT,
                        lineEdit_modifier_client_nom_representant->text());
        record.setValue(YerithDatabaseTableColumn::QUARTIER,
                        lineEdit_modifier_client_quartier->text());
        record.setValue(YerithDatabaseTableColumn::VILLE,
                        lineEdit_modifier_client_ville->text());
        record.setValue(YerithDatabaseTableColumn::PROVINCE_ETAT,
                        lineEdit_modifier_client_province_etat->text());
        record.setValue(YerithDatabaseTableColumn::PAYS,
                        lineEdit_modifier_client_pays->text());
        record.setValue(YerithDatabaseTableColumn::SIEGE_SOCIAL,
                        lineEdit_modifier_client_siege_social->text());
        record.setValue(YerithDatabaseTableColumn::BOITE_POSTALE,
                        lineEdit_modifier_client_boite_postale->text());
        record.setValue(YerithDatabaseTableColumn::EMAIL,
                        lineEdit_modifier_client_email->text());
        record.setValue(YerithDatabaseTableColumn::NUMERO_TELEPHONE_1,
                        lineEdit_modifier_client_numero_telephone_1->text());
        record.setValue(YerithDatabaseTableColumn::NUMERO_TELEPHONE_2,
                        lineEdit_modifier_client_numero_telephone_2->text());
        record.setValue(YerithDatabaseTableColumn::NUMERO_CONTRIBUABLE,
                        lineEdit_modifier_client_numero_contribuable->text());

        if (0 != YerithUtils::getAllWindows())
        {
            YerithPOSUser *aUser = YerithUtils::getAllWindows()->getUser();

            if (0 != aUser && aUser->isManager())
            {
                record.setValue
                (YerithDatabaseTableColumn::DETTE_MAXIMALE_COMPTE_CLIENT,
                 lineEdit_modifier_client_dette_maximale_compte_client->text
                 ().toDouble());
            }
        }

        record.setValue(YerithDatabaseTableColumn::DESCRIPTION_CLIENT,
                        textEdit_modifier_client_description->toPlainText());

        bool success =
                        clientsTableModel->updateRecord(lw->
                                                        lastSelectedItemForModification(),
                                                        record);

        QString retMsg(QObject::tr("Les données du client '%1'").arg
                       (lineEdit_modifier_client_nom_entreprise->text()));

        if (success)
        {
            retMsg.
            append(QObject::tr(" ont été modifiées avec succès !"));

            YerithQMessageBox::information(this,
                                           QObject::tr
                                           ("Yerith-pgi-3.0 ~ admin-modifier-client"),
                                           retMsg);

            _allWindows->_adminListerWindow->
            rendreVisible(SUJET_ACTION_CLIENT);

            rendreInvisible();
        }
        else
        {
            qDebug() << "\t[reason for failing] " <<
                     clientsTableModel->lastError();

            retMsg.
            append(QObject::tr(" n'ont pas pu être modifiées !"));

            YerithQMessageBox::warning(this,
                                       QObject::tr
                                       ("Yerith-pgi-3.0 ~ admin-modifier-client"),
                                       retMsg);
        }
    }
}

bool YerithAdminModifierWindow::modifier_client_check_fields()
{
    bool nom_entreprise =
                    lineEdit_modifier_client_nom_entreprise->checkField();
    bool nom_representant =
                    lineEdit_modifier_client_nom_representant->checkField();

    return nom_entreprise && nom_representant;
}

void YerithAdminModifierWindow::clear_client_all_fields()
{
    lineEdit_modifier_client_reference_client->clearField();
    lineEdit_modifier_client_nom_entreprise->clearField();
    lineEdit_modifier_client_nom_representant->clearField();
    lineEdit_modifier_client_quartier->clear();
    lineEdit_modifier_client_ville->clear();
    lineEdit_modifier_client_province_etat->clear();
    lineEdit_modifier_client_pays->clear();
    lineEdit_modifier_client_siege_social->clear();
    lineEdit_modifier_client_boite_postale->clear();
    lineEdit_modifier_client_email->clear();
    lineEdit_modifier_client_numero_telephone_1->clear();
    lineEdit_modifier_client_numero_telephone_2->clear();
    lineEdit_modifier_client_numero_contribuable->clear();
    textEdit_modifier_client_description->clear();
}
