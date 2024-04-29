
void YerithAdminModifierWindow::setupEditLocalisation()
{
    _windowName = QString("%1 - %2")
    				.arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
                		 QObject::tr("administration ~ modifier ~ localisations"));

    YerithAdminListerWindow *lw = _allWindows->_adminListerWindow;

    YerithSqlTableModel *localisationsTableModel = lw->getCurSearchSqlTableModel();

    if (!localisationsTableModel)
    {
        localisationsTableModel =
                        &_allWindows->getSqlTableModel_localisations();
    }
    else if (localisationsTableModel &&
             !YerithUtils::isEqualCaseInsensitive(localisationsTableModel->
                                                  sqlTableName(),
                                                  YerithDatabase::
                                                  LOCALISATIONS))
    {
        localisationsTableModel =
                        &_allWindows->getSqlTableModel_localisations();
    }

    QSqlRecord record =
                    localisationsTableModel->record(lw->lastSelectedItemForModification());

    lineEdit_modifier_localisation_adresse_ip->setText(GET_SQL_RECORD_DATA
                                                       (record, "adresse_ip"));
    lineEdit_modifier_localisation_nom->setText(GET_SQL_RECORD_DATA
                                                (record, "nom_localisation"));
    lineEdit_modifier_localisation_numero_unique->setText(GET_SQL_RECORD_DATA
                                                          (record,
                                                           "numero_unique"));
    lineEdit_modifier_localisation_quartier->setText(GET_SQL_RECORD_DATA
                                                     (record,
                                                      YerithDatabaseTableColumn::QUARTIER));
    lineEdit_modifier_localisation_ville->setText(GET_SQL_RECORD_DATA
                                                  (record,
                                                   YerithDatabaseTableColumn::VILLE));
    lineEdit_modifier_localisation_province_etat->setText(GET_SQL_RECORD_DATA
                                                          (record,
                                                           "province_etat"));
    lineEdit_modifier_localisation_pays->setText(GET_SQL_RECORD_DATA
                                                 (record,
                                                  YerithDatabaseTableColumn::PAYS));
    lineEdit_modifier_localisation_boite_postale->setText(GET_SQL_RECORD_DATA
                                                          (record,
                                                           YerithDatabaseTableColumn::BOITE_POSTALE));

    QString date_ouverture(GET_SQL_RECORD_DATA(record, "date_ouverture"));
    dateEdit_modifier_localisation_date_ouverture->setDate(GET_DATE_FROM_STRING
                                                           (date_ouverture));
    dateEdit_modifier_localisation_date_ouverture->setEnabled(false);

    lineEdit_modifier_localisation_email->setText(GET_SQL_RECORD_DATA
                                                  (record,
                                                   YerithDatabaseTableColumn::EMAIL));
    lineEdit_modifier_localisation_numero_telephone_1->setText
    (GET_SQL_RECORD_DATA
     (record, YerithDatabaseTableColumn::NUMERO_TELEPHONE_1));
    lineEdit_modifier_localisation_numero_telephone_2->setText
    (GET_SQL_RECORD_DATA
     (record, YerithDatabaseTableColumn::NUMERO_TELEPHONE_2));

    comboBox_modifier_localisation_base_donnees->
    populateComboBoxMissingRawString("nom_dbms", YerithDatabase::DBMS,
                                     GET_SQL_RECORD_DATA(record, "dbms"));

    textEdit_modifier_localisation_description_lieu->setText
    (GET_SQL_RECORD_DATA(record, "description_lieu"));
}

void YerithAdminModifierWindow::modifier_localisation()
{
    //_logger->log("modifier_localisation");
    if (modifier_localisation_check_fields())
    {
        YerithAdminListerWindow *lw = _allWindows->_adminListerWindow;
        YerithSqlTableModel *localisationsTableModel =
                        lw->getCurSearchSqlTableModel();

        if (!localisationsTableModel)
        {
            localisationsTableModel =
                            &_allWindows->getSqlTableModel_localisations();
        }
        else if (localisationsTableModel &&
                 !YerithUtils::isEqualCaseInsensitive(localisationsTableModel->
                                                      sqlTableName(),
                                                      YerithDatabase::
                                                      LOCALISATIONS))
        {
            localisationsTableModel =
                            &_allWindows->getSqlTableModel_localisations();
        }

        QSqlRecord record =
                        localisationsTableModel->record(lw->
                                                        lastSelectedItemForModification());

        record.setValue("adresse_ip",
                        lineEdit_modifier_localisation_adresse_ip->text());
        record.setValue(YerithDatabaseTableColumn::QUARTIER,
                        lineEdit_modifier_localisation_quartier->text());
        record.setValue(YerithDatabaseTableColumn::VILLE,
                        lineEdit_modifier_localisation_ville->text());
        record.setValue("province_etat",
                        lineEdit_modifier_localisation_province_etat->text());
        record.setValue(YerithDatabaseTableColumn::PAYS,
                        lineEdit_modifier_localisation_pays->text());
        record.setValue(YerithDatabaseTableColumn::BOITE_POSTALE,
                        lineEdit_modifier_localisation_boite_postale->text());
        record.setValue(YerithDatabaseTableColumn::EMAIL,
                        lineEdit_modifier_localisation_email->text());
        record.setValue(YerithDatabaseTableColumn::NUMERO_TELEPHONE_1,
                        lineEdit_modifier_localisation_numero_telephone_1->text
                        ());
        record.setValue(YerithDatabaseTableColumn::NUMERO_TELEPHONE_2,
                        lineEdit_modifier_localisation_numero_telephone_2->text
                        ());
        record.setValue("dbms",
                        comboBox_modifier_localisation_base_donnees->
                        currentText());
        record.setValue("description_lieu",
                        textEdit_modifier_localisation_description_lieu->
                        toPlainText());
        record.setValue("numero_unique",
                        lineEdit_modifier_localisation_numero_unique->text());

        bool success =
                        localisationsTableModel->updateRecord(lw->
                                                              lastSelectedItemForModification
                                                              (), record);

        QString
        retMsg(QObject::tr("Les données de la localisation '%1'").arg
               (lineEdit_modifier_localisation_nom->text()));

        if (success)
        {
            retMsg.
            append(QObject::tr(" ont été modifiées avec succès !"));

            YerithQMessageBox::information(this,
                                           QObject::tr
                                           ("Yerith-pgi-3.0 ~ admin-modifier-localisation"),
                                           retMsg);

            _allWindows->_adminListerWindow->
            rendreVisible(SUJET_ACTION_LOCALISATION);

            rendreInvisible();
        }
        else
        {
            qDebug() << "\t[reason for failing] " <<
                     localisationsTableModel->lastError();

            retMsg.
            append(QObject::tr(" n'ont pas pu être modifiées !"));

            YerithQMessageBox::warning(this,
                                       QObject::tr
                                       ("Yerith-pgi-3.0 ~ admin-modifier-localisation"),
                                       retMsg);
        }
    }
}

void YerithAdminModifierWindow::clear_localisation_all_fields()
{
    lineEdit_modifier_localisation_adresse_ip->clearField();
    lineEdit_modifier_localisation_nom->clearField();
    lineEdit_modifier_localisation_quartier->clearField();
    lineEdit_modifier_localisation_ville->clearField();
    lineEdit_modifier_localisation_province_etat->clearField();
    lineEdit_modifier_localisation_pays->clearField();
    lineEdit_modifier_localisation_boite_postale->clearField();
    dateEdit_modifier_localisation_date_ouverture->clear();
    lineEdit_modifier_localisation_email->clearField();
    lineEdit_modifier_localisation_numero_telephone_1->clearField();
    lineEdit_modifier_localisation_numero_telephone_2->clearField();
    comboBox_modifier_localisation_base_donnees->clearField();
    textEdit_modifier_localisation_description_lieu->clear();
}

bool YerithAdminModifierWindow::modifier_localisation_check_fields()
{
    bool nom = lineEdit_modifier_localisation_nom->checkField();
    bool quartier = lineEdit_modifier_localisation_quartier->checkField();
    bool ville = lineEdit_modifier_localisation_ville->checkField();

    return nom && quartier && ville;
}
