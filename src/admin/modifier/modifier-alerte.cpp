
void YerithAdminModifierWindow::setupEditAlerte()
{
    _windowName = QString("%1 - %2")
                			.arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
                				 QObject::tr("administration ~ modifier ~ alertes"));

    YerithAdminListerWindow *lw = _allWindows->_adminListerWindow;

    YerithSqlTableModel *alertesTableModel = lw->getCurSearchSqlTableModel();

    if (!alertesTableModel)
    {
        alertesTableModel = &_allWindows->getSqlTableModel_alertes();
    }
    else if (alertesTableModel &&
             !YerithUtils::isEqualCaseInsensitive(alertesTableModel->
                                                  sqlTableName(),
                                                  YerithDatabase::ALERTES))
    {
        alertesTableModel = &_allWindows->getSqlTableModel_alertes();
    }

    QSqlRecord record =
                    alertesTableModel->record(lw->lastSelectedItemForModification());

    lineEdit_modifier_alerte_designation_alerte->setText(GET_SQL_RECORD_DATA
                                                         (record,
                                                          YerithDatabaseTableColumn::DESIGNATION_ALERTE));
    textEdit_modifier_alerte_message->setText(GET_SQL_RECORD_DATA
                                              (record,
                                               YerithDatabaseTableColumn::MESSAGE_ALERTE));

    QString
    date_debut(GET_SQL_RECORD_DATA
               (record, YerithDatabaseTableColumn::DATE_DEBUT));
    QString
    date_fin(GET_SQL_RECORD_DATA
             (record, YerithDatabaseTableColumn::DATE_FIN));

    dateEdit_modifier_alerte_date_debut->setDate(GET_DATE_FROM_STRING
                                                 (date_debut));
    dateEdit_modifier_alerte_date_fin->setDate(GET_DATE_FROM_STRING
                                               (date_fin));

    radioButton_modifier_alerte_periode_temps->setEnabled(false);
    radioButton_modifier_alerte_quantite->setEnabled(false);

    if (GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::QUANTITE).toInt
            () > -1)
    {
        lineEdit_modifier_alerte_quantite->setText(GET_SQL_RECORD_DATA
                                                   (record,
                                                    YerithDatabaseTableColumn::QUANTITE));
        radioButton_modifier_alerte_quantite->setChecked(true);
        radioButton_modifier_alerte_periode_temps->setChecked(false);
    }
    else
    {
        lineEdit_modifier_alerte_quantite->setText("-1");
        radioButton_modifier_alerte_periode_temps->setChecked(true);
        radioButton_modifier_alerte_quantite->setChecked(false);
    }

    lineEdit_modifier_alerte_designation_article->setText(GET_SQL_RECORD_DATA
                                                          (record,
                                                           YerithDatabaseTableColumn::DESIGNATION));
    lineEdit_modifier_alerte_designation_article->setEnabled(false);

    lineEdit_modifier_alerte_id_destinataire->setText(GET_SQL_RECORD_DATA
                                                      (record,
                                                       YerithDatabaseTableColumn::DESTINATAIRE));
    lineEdit_modifier_alerte_id_destinataire->setEnabled(false);

    lineEdit_modifier_alerte_nom_destinataire->setText(GET_SQL_RECORD_DATA
                                                       (record,
                                                        YerithDatabaseTableColumn::NOM_COMPLET_DESTINATAIRE));

    comboBox_modifier_alerte_condition->addItem(GET_SQL_RECORD_DATA
                                                (record,
                                                 YerithDatabaseTableColumn::CONDITION_ALERTE));
    comboBox_modifier_alerte_condition->setEnabled(false);
}

void YerithAdminModifierWindow::modifier_alerte()
{
    //_logger->log("modifier_alerte");
    YerithAdminListerWindow *lw = _allWindows->_adminListerWindow;
    YerithSqlTableModel *alertesTableModel = lw->getCurSearchSqlTableModel();

    if (!alertesTableModel)
    {
        alertesTableModel = &_allWindows->getSqlTableModel_alertes();
    }
    else if (alertesTableModel &&
             !YerithUtils::isEqualCaseInsensitive(alertesTableModel->
                                                  sqlTableName(),
                                                  YerithDatabase::ALERTES))
    {
        alertesTableModel = &_allWindows->getSqlTableModel_alertes();
    }

    QSqlRecord record =
                    alertesTableModel->record(lw->lastSelectedItemForModification());

    if (modifier_alerte_check_fields
            (GET_SQL_RECORD_DATA
             (record, YerithDatabaseTableColumn::MESSAGE_ALERTE)))
    {
        record.setValue(YerithDatabaseTableColumn::MESSAGE_ALERTE,
                        textEdit_modifier_alerte_message->toPlainText());

        bool success =
                        alertesTableModel->updateRecord(lw->
                                                        lastSelectedItemForModification(),
                                                        record);

        QString retMsg(QObject::tr("Le message de l'alerte '%1' ").arg
                       (lineEdit_modifier_alerte_designation_alerte->text()));

        if (success)
        {
            retMsg.
            append(QObject::tr("a été modifiées avec succès !"));

            YerithQMessageBox::information(this,
                                           QObject::tr
                                           ("Yerith-pgi-3.0 ~ administration ~ modifier ~ alerte"),
                                           retMsg);

            _allWindows->_adminListerWindow->
            rendreVisible(SUJET_ACTION_ALERTE);
            rendreInvisible();
        }
        else
        {
            qDebug() << "\t[reason for failing] " <<
                     alertesTableModel->lastError();

            retMsg.append(QObject::tr(" n'a pas pu être modifiées !"));

            YerithQMessageBox::warning(this,
                                       QObject::tr
                                       ("Yerith-pgi-3.0 ~ administration ~ modifier ~ alerte"),
                                       retMsg);
        }
    }
}



bool YerithAdminModifierWindow::modifier_alerte_check_fields(QString
                                                             previousAlertText)
{
    QString currentText(textEdit_modifier_alerte_message->toPlainText());
    bool modified = (previousAlertText != currentText);
    if (!modified)
    {
        YerithQMessageBox::information(this,
                                       QObject::tr
                                       ("Yerith-pgi-3.0 ~ administration ~ modifier ~ alerte"),
                                       QObject::tr
                                       ("Le message de l'alerte n'a pas changé!"));
        return false;
    }

    return true;
}

void YerithAdminModifierWindow::clear_alerte_all_fields()
{
    textEdit_modifier_alerte_message->clear();
    lineEdit_modifier_alerte_designation_article->clear();
    comboBox_modifier_alerte_condition->clear();
    lineEdit_modifier_alerte_id_destinataire->clear();
}
