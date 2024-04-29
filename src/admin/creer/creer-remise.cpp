
void YerithAdminCreateWindow::populateRemiseComboBoxes()
{
    _logger->log("populateRemiseComboBoxes");
}


void YerithAdminCreateWindow::creer_remise_check_fields_entry()
{
    lineEdit_creer_remise_nom->checkField();
    lineEdit_creer_remise_designation_article->checkField();
}


bool YerithAdminCreateWindow::creer_remise_check_fields()
{
    bool remise_nom = lineEdit_creer_remise_nom->checkField();
    bool produit = lineEdit_creer_remise_designation_article->checkField();

    bool check = remise_nom && produit;


    if (dateEdit_creer_remise_date_debut->date() < GET_CURRENT_DATE)
    {
        dateEdit_creer_remise_date_debut->
        setPalette(YerithUtils::YERITH_RED_PALETTE);
        dateEdit_creer_remise_date_fin->
        setPalette(YerithUtils::YERITH_RED_PALETTE);

        QString pMsg(QObject::tr
                     ("Le paramètre 'début' de la remise ne doit pas être dans le passé !"));

        YerithQMessageBox::information(this,
                                       QObject::tr
                                       ("Yerith-pgi-3.0 - créer ~ remise"),
                                       pMsg);
        return false;
    }

    bool periode =
                    (dateEdit_creer_remise_date_debut->date() <=
                     dateEdit_creer_remise_date_fin->date());

    if (!periode)
    {
        dateEdit_creer_remise_date_debut->
        setPalette(YerithUtils::YERITH_RED_PALETTE);
        dateEdit_creer_remise_date_fin->
        setPalette(YerithUtils::YERITH_RED_PALETTE);

        QString pMsg(QObject::tr
                     ("Le paramètre 'début' de la remise doit être avant le paramètre 'fin' !"));

        YerithQMessageBox::information(this,
                                       QObject::tr
                                       ("Yerith-pgi-3.0 - créer ~ remise"),
                                       pMsg);
        return false;
    }

    return check;
}


bool YerithAdminCreateWindow::creer_remise()
{
    if (creer_remise_check_fields())
    {
        QString retMsg(QObject::tr("Une remise dénomée '"));

        YerithSqlTableModel &remiseTableModel =
                        _allWindows->getSqlTableModel_remises();

        remiseTableModel.yerithSetFilter_WITH_where_clause(QString
                                                           ("LOWER(%1) = LOWER('%2')").
                                                           arg
                                                           (YerithDatabaseTableColumn::
                                                            DESIGNATION_REMISE,
                                                            lineEdit_creer_remise_nom->
                                                            text()));

        //qDebug() << "++ filter: " << remiseTableModel.filter();

        int remisesTableModelRowCount = remiseTableModel.easySelect();

        //qDebug() << "++ result remisesTableModelRowCount : " << remisesTableModelRowCount ;

        if (remisesTableModelRowCount > 0)
        {
            QSqlRecord record = remiseTableModel.record(0);

            QString
            duplicateRemise(GET_SQL_RECORD_DATA
                            (record,
                             YerithDatabaseTableColumn::DESIGNATION_REMISE));

            //qDebug() << "++ duplicate remise: " << duplicateAlerte;

            retMsg.append(QObject::tr("%1' existe déjà ! ").
                          arg(duplicateRemise));

            YerithQMessageBox::warning(this,
                                       QObject::tr
                                       ("Yerith-pgi-3.0 ~ administration ~ créer ~ remise"),
                                       retMsg);

            remiseTableModel.resetFilter();

            return false;
        }

        remiseTableModel.resetFilter();

        QSqlRecord record = remiseTableModel.record();

        YerithSqlTableModel &stocksTableModel =
                        _allWindows->getSqlTableModel_stocks();

        stocksTableModel.yerithSetFilter_WITH_where_clause(GENERATE_SQL_IS_STMT
                                                           (YerithDatabaseTableColumn::
                                                            DESIGNATION,
                                                            lineEdit_creer_remise_designation_article->
                                                            text()));

        int stocksTableModelRowCount = stocksTableModel.easySelect();
        if (stocksTableModelRowCount <= 0)
        {
            YerithQMessageBox::warning(this,
                                       QObject::tr
                                       ("Yerith-pgi-3.0 ~ administration ~ créer ~ remise"),
                                       QObject::tr
                                       ("Il n'est pas possible de créer une remise "
                                        "sur un stock déjà écoulé !"));

            return false;
        }

        QSqlRecord stocksRecord = stocksTableModel.record(0);

        record.setValue(YerithDatabaseTableColumn::DESIGNATION_REMISE,
                        lineEdit_creer_remise_nom->text());
        record.setValue(YerithDatabaseTableColumn::DESIGNATION,
                        lineEdit_creer_remise_designation_article->text());

        YerithPOSUser *currentUser = _allWindows->getUser();

        if (0 != currentUser)
        {
            record.setValue
            (YerithDatabaseTableColumn::NOM_COMPLET_CREATEUR_REMISE_ALERTE,
             currentUser->nom_complet());
        }

        record.setValue(YerithDatabaseTableColumn::STOCKS_ID,
                        GET_SQL_RECORD_DATA(stocksRecord,
                                            YerithDatabaseTableColumn::ID));
        record.setValue(YerithDatabaseTableColumn::REMISE_RESOLUE, 0);

        record.setValue(YerithDatabaseTableColumn::REMISE_POURCENTAGE,
                        spinBox_remise_pourcentage->value());

        record.setValue(YerithDatabaseTableColumn::DATE_DEBUT,
                        dateEdit_creer_remise_date_debut->date());
        record.setValue(YerithDatabaseTableColumn::DATE_FIN,
                        dateEdit_creer_remise_date_fin->date());

        record.setValue(YerithDatabaseTableColumn::REMISE_NOTES,
                        textEdit_creer_remise_message->toPlainText());
        record.setValue(YerithDatabaseTableColumn::DATE_CREATION,
                        GET_CURRENT_DATE);

        bool success =
        		remiseTableModel
					.insertNewRecord(record,
									_allWindows->_adminCreateWindow,
									"src/admin/creer/creer-remise.cpp",
									172,
									lineEdit_creer_remise_nom->text());

        if (!success)
        {
            retMsg.append(QObject::tr("%1' n'a pas pu être créer !").arg
                          (lineEdit_creer_remise_nom->text()));

            YerithQMessageBox::warning(this,
                                       QObject::tr
                                       ("Yerith-pgi-3.0 - créer"), retMsg);
            return false;
        }

        retMsg.append(QObject::tr("%1' a été créer avec succès !").arg
                      (lineEdit_creer_remise_nom->text()));

        YerithQMessageBox::information(this,
                                       QObject::tr
                                       ("Yerith-pgi-3.0 - créer"), retMsg);

        clear_remise_all_fields();
        rendreInvisible();

        stocksTableModel.resetFilter();

        _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_REMISE);

        return true;
    }
    else
    {
        YerithQMessageBox::information(this,
                                       QObject::tr
                                       ("Yerith-pgi-3.0 ~ créer"),
                                       QObject::tr
                                       ("BIEN VOULOIR REMPLIR TOUS LES "
                                        "CHAMPS DE TEXTES OBLIGATOIRES !"));

        return false;
    }

    return false;
}


void YerithAdminCreateWindow::clear_remise_all_fields()
{
    lineEdit_creer_remise_nom->clearField();
    lineEdit_creer_remise_designation_article->clearField();

    lineEdit_creer_remise_quantite_en_stock->clear();
    lineEdit_creer_remise_date_peremption->clear();

    textEdit_creer_remise_message->clearField();

    dateEdit_creer_remise_date_debut->reset();
    dateEdit_creer_remise_date_fin->reset();

    dateEdit_creer_remise_date_debut->
    setPalette(YerithUtils::YERITH_WHITE_PALETTE);
    dateEdit_creer_remise_date_fin->
    setPalette(YerithUtils::YERITH_WHITE_PALETTE);
}
