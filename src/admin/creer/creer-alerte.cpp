bool YerithAdminCreateWindow::creer_alerte()
{
    if (creer_alerte_check_fields())
    {
        QString retMsg(QObject::tr("Une alerte au nom de '"));

        YerithSqlTableModel &alertesTableModel =
                        _allWindows->getSqlTableModel_alertes();

        alertesTableModel.yerithSetFilter_WITH_where_clause(QString
                                                            ("LOWER(designation_alerte) = LOWER('%1')").
                                                            arg
                                                            (lineEdit_creer_alerte_nom->
                                                             text()));

        //qDebug() << "++ filter: " << alertesTableModel.filter();

        int alertesTableModelRowCount = alertesTableModel.easySelect();

        //qDebug() << "++ result alertesTableModelRowCount : " << alertesTableModelRowCount ;

        if (alertesTableModelRowCount > 0)
        {
            QSqlRecord record = alertesTableModel.record(0);
            QString
            duplicateAlerte(GET_SQL_RECORD_DATA
                            (record,
                             YerithDatabaseTableColumn::DESIGNATION_ALERTE));

            //qDebug() << "++ duplicate alerte: " << duplicateAlerte;

            retMsg.append(QObject::tr("%1' existe déjà ! ").
                          arg(duplicateAlerte));

            YerithQMessageBox::warning(this,
                                       QObject::tr
                                       ("Yerith-pgi-3.0 ~ administration ~ créer ~ alerte"),
                                       retMsg);

            alertesTableModel.resetFilter("src/admin/creer/creer-alerte.cpp");

            return false;
        }

        alertesTableModel.resetFilter("src/admin/creer/creer-alerte.cpp");

        QSqlRecord record = alertesTableModel.record();

        YerithSqlTableModel &stocksTableModel =
                        _allWindows->getSqlTableModel_stocks();

        stocksTableModel.yerithSetFilter_WITH_where_clause(GENERATE_SQL_IS_STMT
                                                           (YerithDatabaseTableColumn::
                                                            DESIGNATION,
                                                            comboBox_creer_alerte_designation->
                                                            currentText()));

        int stocksTableModelRowCount = stocksTableModel.easySelect();

        if (stocksTableModelRowCount <= 0)
        {
            YerithQMessageBox::warning(this,
                                       QObject::tr
                                       ("Yerith-pgi-3.0 ~ administration ~ créer ~ alerte"),
                                       QObject::tr
                                       ("Il n'est pas possible de créer une alerte "
                                        "sur un stock déjà écoulé !"));

            return false;
        }

        QSqlRecord stocksRecord = stocksTableModel.record(0);

        record.setValue(YerithDatabaseTableColumn::ID,
                        YerithERPWindows::getNextIdSqlTableModel_alertes());

        record.setValue(YerithDatabaseTableColumn::DESIGNATION_ALERTE,
                        lineEdit_creer_alerte_nom->text());

        record.setValue(YerithDatabaseTableColumn::DESTINATAIRE,
                        comboBox_creer_alerte_destinataire->currentText());

        record.setValue(YerithDatabaseTableColumn::NOM_COMPLET_DESTINATAIRE,
                        lineEdit_creer_alerte_nom_destinataire->text());

        record.setValue(YerithDatabaseTableColumn::DESIGNATION,
                        comboBox_creer_alerte_designation->currentText());

        record.setValue(YerithDatabaseTableColumn::STOCKS_ID,
                        GET_SQL_RECORD_DATA(stocksRecord,
                                            YerithDatabaseTableColumn::ID));

        record.setValue(YerithDatabaseTableColumn::ALERTE_RESOLUE, 0);


        QString quantite = lineEdit_creer_alerte_quantite->text();

        if (radioButton_creer_alerte_quantite->isChecked())
        {
            record.setValue(YerithDatabaseTableColumn::QUANTITE,
            				quantite);

            comboBox_creer_alerte_condition
				->saveRawCurrentValueToDatabase(YerithDatabaseTableColumn::CONDITION_ALERTE,
												record);
        }
        else if (radioButton_creer_alerte_periode_temps->isChecked())
        {
            quantite = "-1";

            record.setValue(YerithDatabaseTableColumn::QUANTITE,
            				quantite);

            record.setValue(YerithDatabaseTableColumn::DATE_DEBUT,
                            dateEdit_creer_alerte_date_debut->date());

            record.setValue(YerithDatabaseTableColumn::DATE_FIN,
                            dateEdit_creer_alerte_date_fin->date());
        }

        record.setValue(YerithDatabaseTableColumn::MESSAGE_ALERTE,
                        textEdit_creer_alerte_message->toPlainText());

        record.setValue(YerithDatabaseTableColumn::STATUT_RESOLU, false);

        record.setValue(YerithDatabaseTableColumn::DATE_CREATION,
                        GET_CURRENT_DATE);

        bool success =
        		alertesTableModel
					.insertNewRecord(record,
									 _allWindows->_adminCreateWindow,
									 "src/admin/creer/creer-alerte.cpp",
									 129,
									 lineEdit_creer_alerte_nom->text());

        if (!success)
        {
            retMsg.append(QObject::tr("%1' n'a pas pu être créer !").arg
                          (lineEdit_creer_alerte_nom->text()));

            YerithQMessageBox::warning(this,
                                       QObject::tr
                                       ("Yerith-pgi-3.0 - créer"), retMsg);
            return false;
        }

        retMsg.append(QObject::tr("%1' a été créer avec succès !").arg
                      (lineEdit_creer_alerte_nom->text()));

        YerithQMessageBox::information(this,
                                       QObject::tr
                                       ("Yerith-pgi-3.0 - créer"), retMsg);

        clear_alerte_all_fields();
        rendreInvisible();

        stocksTableModel.resetFilter("src/admin/creer/creer-alerte.cpp");

        _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_ALERTE);

        return true;
    }
    else
    {
        YerithQMessageBox::information(this,
                                       QObject::tr
                                       ("Yerith-pgi-3.0 ~ administration ~ créer ~ alerte"),
                                       QObject::tr
                                       ("BIEN VOULOIR REMPLIR TOUS LES "
                                        "CHAMPS DE TEXTES OBLIGATOIRES !"));

        return false;
    }

    return false;
}


void YerithAdminCreateWindow::populateAlerteComboBoxes()
{
    _logger->log("populateAlerteComboBoxes");

    QString
    aConditionStr(QString("NOT (%1 = '%2')").arg
                  (YerithDatabaseTableColumn::ROLE,
                   QString::number(YerithUtils::ROLE_ADMINISTRATEUR)));

    comboBox_creer_alerte_destinataire->populateComboBoxRawString
    (YerithDatabase::USERS, YerithDatabaseTableColumn::NOM_UTILISATEUR,
     aConditionStr);

    comboBox_creer_alerte_condition->
    populateComboBoxRawString(YerithDatabase::CONDITIONS_ALERTES,
                              YerithDatabaseTableColumn::CONDITION_ALERTE);
}


void YerithAdminCreateWindow::creer_alerte_check_fields_entry()
{
    comboBox_creer_alerte_designation->checkField();
    comboBox_creer_alerte_destinataire->checkField();

    lineEdit_creer_alerte_nom->checkField();
    textEdit_creer_alerte_message->checkField();

    if (!radioButton_creer_alerte_quantite->isChecked() &&
            !radioButton_creer_alerte_periode_temps->isChecked())
    {
        comboBox_creer_alerte_condition->checkField();
        lineEdit_creer_alerte_quantite->
        setPalette(YerithUtils::YERITH_RED_PALETTE);
        dateEdit_creer_alerte_date_debut->
        setPalette(YerithUtils::YERITH_RED_PALETTE);
        dateEdit_creer_alerte_date_fin->
        setPalette(YerithUtils::YERITH_RED_PALETTE);
    }
    else
    {
        comboBox_creer_alerte_condition->checkField();
        lineEdit_creer_alerte_quantite->
        setPalette(YerithUtils::YERITH_WHITE_PALETTE);
        dateEdit_creer_alerte_date_debut->
        setPalette(YerithUtils::YERITH_WHITE_PALETTE);
        dateEdit_creer_alerte_date_fin->
        setPalette(YerithUtils::YERITH_WHITE_PALETTE);
    }

    if (radioButton_creer_alerte_quantite->isChecked())
    {
        dateEdit_creer_alerte_date_debut->
        setPalette(YerithUtils::YERITH_WHITE_PALETTE);
        dateEdit_creer_alerte_date_fin->
        setPalette(YerithUtils::YERITH_WHITE_PALETTE);

        lineEdit_creer_alerte_quantite->checkField();

        comboBox_creer_alerte_condition->checkField();
    }
    else if (radioButton_creer_alerte_periode_temps->isChecked())
    {
        comboBox_creer_alerte_condition->
        setPalette(YerithUtils::YERITH_WHITE_PALETTE);
        lineEdit_creer_alerte_quantite->
        setPalette(YerithUtils::YERITH_WHITE_PALETTE);

        if (dateEdit_creer_alerte_date_debut->date() < GET_CURRENT_DATE)
        {
            dateEdit_creer_alerte_date_debut->
            setPalette(YerithUtils::YERITH_RED_PALETTE);
            dateEdit_creer_alerte_date_fin->
            setPalette(YerithUtils::YERITH_RED_PALETTE);
        }

        bool periode =
                        (dateEdit_creer_alerte_date_debut->date() <=
                         dateEdit_creer_alerte_date_fin->date());

        if (!periode)
        {
            dateEdit_creer_alerte_date_debut->
            setPalette(YerithUtils::YERITH_RED_PALETTE);
            dateEdit_creer_alerte_date_fin->
            setPalette(YerithUtils::YERITH_RED_PALETTE);
        }
    }
}


bool YerithAdminCreateWindow::creer_alerte_check_fields()
{
    bool alerte_nom = lineEdit_creer_alerte_nom->checkField();
    bool produit = comboBox_creer_alerte_designation->checkField();
    bool message = textEdit_creer_alerte_message->checkField();
    bool destinataire = comboBox_creer_alerte_destinataire->checkField();

    bool check = alerte_nom && produit && message && destinataire;

    if (!radioButton_creer_alerte_quantite->isChecked() &&
            !radioButton_creer_alerte_periode_temps->isChecked())
    {
        QString
        aMsg(QObject::tr
             ("Veuillez choisir comme paramètre de l'alerte "
              "soit la 'quantité', soit la 'période de temps' !"));

        YerithQMessageBox::information(this,
                                       QObject::tr
                                       ("Yerith-pgi-3.0 - créer ~ alerte"),
                                       aMsg);

        comboBox_creer_alerte_condition->checkField();

        lineEdit_creer_alerte_quantite->
        setPalette(YerithUtils::YERITH_RED_PALETTE);
        dateEdit_creer_alerte_date_debut->
        setPalette(YerithUtils::YERITH_RED_PALETTE);
        dateEdit_creer_alerte_date_fin->
        setPalette(YerithUtils::YERITH_RED_PALETTE);

        return false;
    }
    else
    {
        comboBox_creer_alerte_condition->checkField();
        lineEdit_creer_alerte_quantite->
        setPalette(YerithUtils::YERITH_WHITE_PALETTE);
        dateEdit_creer_alerte_date_debut->
        setPalette(YerithUtils::YERITH_WHITE_PALETTE);
        dateEdit_creer_alerte_date_fin->
        setPalette(YerithUtils::YERITH_WHITE_PALETTE);
    }

    if (radioButton_creer_alerte_quantite->isChecked())
    {
        dateEdit_creer_alerte_date_debut->
        setPalette(YerithUtils::YERITH_WHITE_PALETTE);
        dateEdit_creer_alerte_date_fin->
        setPalette(YerithUtils::YERITH_WHITE_PALETTE);

        bool quantite = lineEdit_creer_alerte_quantite->checkField();
        if (quantite)
        {
            int q = lineEdit_creer_alerte_quantite->text().toInt();
            if (q < 0)
            {
                QString qMsg(QObject::tr
                             ("La quantité doit être supérieure à 0 !"));

                YerithQMessageBox::information(this,
                                               QObject::tr
                                               ("Yerith-pgi-3.0 - créer-alerte"),
                                               qMsg);
                return false;
            }
        }

        bool condition = comboBox_creer_alerte_condition->checkField();

        check = check && quantite && condition;
    }
    else if (radioButton_creer_alerte_periode_temps->isChecked())
    {
        comboBox_creer_alerte_condition->
        setPalette(YerithUtils::YERITH_WHITE_PALETTE);
        lineEdit_creer_alerte_quantite->
        setPalette(YerithUtils::YERITH_WHITE_PALETTE);

        if (dateEdit_creer_alerte_date_debut->date() < GET_CURRENT_DATE)
        {
            dateEdit_creer_alerte_date_debut->
            setPalette(YerithUtils::YERITH_RED_PALETTE);
            dateEdit_creer_alerte_date_fin->
            setPalette(YerithUtils::YERITH_RED_PALETTE);

            QString pMsg(QObject::tr
                         ("Le paramètre 'début' de l'alerte ne doit pas être dans le passé !"));

            YerithQMessageBox::information(this,
                                           QObject::tr
                                           ("Yerith-pgi-3.0 - créer ~ alerte"),
                                           pMsg);
            return false;
        }

        bool periode =
                        (dateEdit_creer_alerte_date_debut->date() <=
                         dateEdit_creer_alerte_date_fin->date());

        if (!periode)
        {
            dateEdit_creer_alerte_date_debut->
            setPalette(YerithUtils::YERITH_RED_PALETTE);
            dateEdit_creer_alerte_date_fin->
            setPalette(YerithUtils::YERITH_RED_PALETTE);

            QString pMsg(QObject::tr
                         ("Le paramètre 'début' de l'alerte doit être avant le paramètre 'fin' !"));

            YerithQMessageBox::information(this,
                                           QObject::tr
                                           ("Yerith-pgi-3.0 - créer ~ alerte"),
                                           pMsg);
            return false;
        }

        check = check && periode;
    }

    return check;
}


void YerithAdminCreateWindow::clear_alerte_all_fields()
{
    lineEdit_creer_alerte_nom->clearField();
    lineEdit_creer_alerte_quantite->clearField();
    comboBox_creer_alerte_designation->clearField();

    comboBox_creer_alerte_destinataire->resetYerithComboBox();

    lineEdit_creer_alerte_stock_dalerte->clear();
    lineEdit_creer_alerte_nom_destinataire->clear();
    lineEdit_creer_alerte_quantite_en_stock->clear();
    lineEdit_creer_alerte_date_peremption->clear();

    textEdit_creer_alerte_message->clearField();

    comboBox_creer_alerte_condition->clearField();

    dateEdit_creer_alerte_date_debut->reset();
    dateEdit_creer_alerte_date_fin->reset();

    dateEdit_creer_alerte_date_debut->
    setPalette(YerithUtils::YERITH_WHITE_PALETTE);
    dateEdit_creer_alerte_date_fin->
    setPalette(YerithUtils::YERITH_WHITE_PALETTE);
}
