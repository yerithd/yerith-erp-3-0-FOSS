

void YerithAdminModifierWindow::setupEditCompteBancaire()
{
    _windowName = QString("%1 - %2")
    				.arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
                		 QObject::tr("administration ~ modifier ~ comptes bancaires"));

    YerithAdminListerWindow *lw = _allWindows->_adminListerWindow;

    YerithSqlTableModel *comptesBancairesTableModel = lw->getCurSearchSqlTableModel();

    if (0 == comptesBancairesTableModel)
    {
        comptesBancairesTableModel =
                        &_allWindows->getSqlTableModel_comptes_bancaires();
    }
    else if (false ==
             YerithUtils::isEqualCaseInsensitive(comptesBancairesTableModel->
                                                 sqlTableName(),
                                                 YerithDatabase::
                                                 COMPTES_BANCAIRES))
    {
        comptesBancairesTableModel =
                        &_allWindows->getSqlTableModel_comptes_bancaires();
    }

    QSqlRecord record =
                    comptesBancairesTableModel->record(lw->
                                                       lastSelectedItemForModification());

    lineEdit_modifier_compte_bancaire_reference_du_compte_bancaire->setText
    (GET_SQL_RECORD_DATA
     (record, YerithDatabaseTableColumn::REFERENCE_DU_COMPTE_BANCAIRE));

    lineEdit_modifier_compte_bancaire_intitule_du_compte_bancaire->setText
    (GET_SQL_RECORD_DATA
     (record, YerithDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE));

    lineEdit_modifier_compte_bancaire_institut_bancaire->setText
    (GET_SQL_RECORD_DATA
     (record, YerithDatabaseTableColumn::INSTITUT_BANCAIRE));

    textEdit_modifier_compte_bancaire_description_du_compte->setText
    (GET_SQL_RECORD_DATA
     (record, YerithDatabaseTableColumn::DESCRIPTION_DU_COMPTE_BANCAIRE));
}


void YerithAdminModifierWindow::modifier_compte_bancaire()
{
    if (modifier_compte_bancaire_check_fields())
    {
        YERITH_ERP_3_0_START_DATABASE_TRANSACTION;
        modifier_compte_bancaire_main();
        YERITH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;
    }
}


void YerithAdminModifierWindow::modifier_compte_bancaire_main()
{
    YerithAdminListerWindow *lw = _allWindows->_adminListerWindow;

    YerithSqlTableModel *comptesBancairesTableModel = lw->getCurSearchSqlTableModel();

    if (0 == comptesBancairesTableModel)
    {
        comptesBancairesTableModel =  &_allWindows->getSqlTableModel_comptes_bancaires();
    }
    else if (0 != comptesBancairesTableModel &&
             !YerithUtils::isEqualCaseInsensitive(comptesBancairesTableModel->sqlTableName(),
                     YerithDatabase::COMPTES_BANCAIRES))
    {
        comptesBancairesTableModel =  &_allWindows->getSqlTableModel_comptes_bancaires();
    }

    QSqlRecord record = comptesBancairesTableModel->record(lw->lastSelectedItemForModification());

    QString old_INTITULE_DU_CompteBancaire(
    			GET_SQL_RECORD_DATA(record,
    								YerithDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE));

    QString new_INTITULE_DU_CompteBancaire(lineEdit_modifier_compte_bancaire_intitule_du_compte_bancaire->text());


    QSqlQuery CHECK_intitule_du_compte_bancaire_QUERY;

    QString CHECK_WHETHER_intitule_du_compte_bancaire_ALREADY_EXISTS_query =
    		QString("select %1 from %2 where %3='%4';")
				.arg(YerithDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE,
					 YerithDatabase::COMPTES_BANCAIRES,
					 YerithDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE,
					 new_INTITULE_DU_CompteBancaire);

    int rowCount =
    		YerithUtils::execQuery(CHECK_intitule_du_compte_bancaire_QUERY,
    							   CHECK_WHETHER_intitule_du_compte_bancaire_ALREADY_EXISTS_query);

    if (rowCount > 0)
    {
    	QString retMsg_ALREADY_EXISTS(
    			QObject::tr("Un compte bancaire '%1' existe déjà !")
					.arg(new_INTITULE_DU_CompteBancaire));

        YerithQMessageBox::warning(this,
                                   QObject::tr("Yerith-pgi-3.0 ~ admin-modifier-ligne budgétaire"),
								    retMsg_ALREADY_EXISTS);

        comptesBancairesTableModel->resetFilter();

        return ;
    }


    record.setValue(YerithDatabaseTableColumn::REFERENCE_DU_COMPTE_BANCAIRE,
    		lineEdit_modifier_compte_bancaire_reference_du_compte_bancaire->text());

    record.setValue(YerithDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE,
    				new_INTITULE_DU_CompteBancaire);

    record.setValue(YerithDatabaseTableColumn::INSTITUT_BANCAIRE,
    		lineEdit_modifier_compte_bancaire_institut_bancaire->text());

    record.setValue(YerithDatabaseTableColumn::DESCRIPTION_CATEGORIE,
    				textEdit_modifier_categorie_description->toPlainText());

    bool success_COMPTE_bancaire_Table = comptesBancairesTableModel
    								->updateRecord(lw->lastSelectedItemForModification(), record);

    if (success_COMPTE_bancaire_Table)
    {
        //Handling of table "lignes_budgetaires"
      QString lignes_budgetaires_Query(QString("UPDATE %1 SET %2='%3' WHERE %4='%5'")
                                           .arg(YerithDatabase::LIGNES_BUDGETAIRES,
                                        		YerithDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE,
												new_INTITULE_DU_CompteBancaire,
												YerithDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE,
												old_INTITULE_DU_CompteBancaire));

        YerithUtils::execQuery(lignes_budgetaires_Query, 0);
    }

    if (success_COMPTE_bancaire_Table)
    {
        ////Handling of table "PAIEMENTS"
        QString PAIEMENTS_Query(QString("UPDATE %1 SET %2='%3' WHERE %4='%5'")
                                     .arg(YerithDatabase::PAIEMENTS,
                                          YerithDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE,
										  new_INTITULE_DU_CompteBancaire,
  										  YerithDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE,
  										  old_INTITULE_DU_CompteBancaire));

        YerithUtils::execQuery(PAIEMENTS_Query, 0);
    }


    QString retMsg(QObject::tr("LES DONNÉES DU COMPTE BANCAIRE '%1'")
                       .arg(lineEdit_modifier_compte_bancaire_reference_du_compte_bancaire->text()));

    if (success_COMPTE_bancaire_Table)
    {
        retMsg.append(QObject::tr(" ont été modifiées avec succès !"));

        YerithQMessageBox::information(this,
                                 QObject::tr("Yerith-pgi-3.0 ~ admin-modifier-catégorie"),
                                 retMsg);

        _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_COMPTE_BANCAIRE);
         rendreInvisible();
    }
    else
    {
        qDebug() << "\t[reason for failing] " << comptesBancairesTableModel->lastError();

        retMsg.append(QObject::tr(" n'ont pas pu être modifiées !"));

        YerithQMessageBox::warning(this,
                             QObject::tr("Yerith-pgi-3.0 ~ admin-modifier-catégorie"),
                             retMsg);
    }
}


bool YerithAdminModifierWindow::modifier_compte_bancaire_check_fields()
{
    bool reference_du_compte_bancaire =
                    lineEdit_modifier_compte_bancaire_reference_du_compte_bancaire->checkField
                    ();
    bool intitule_du_compte_bancaire =
                    lineEdit_modifier_compte_bancaire_intitule_du_compte_bancaire->checkField
                    ();
    bool institut_bancaire =
                    lineEdit_modifier_compte_bancaire_institut_bancaire->checkField();

    return reference_du_compte_bancaire &&
           intitule_du_compte_bancaire  &&
		   institut_bancaire;
}

void YerithAdminModifierWindow::clear_compte_bancaire_all_fields()
{
    lineEdit_modifier_compte_bancaire_institut_bancaire->clearField();
    lineEdit_modifier_compte_bancaire_intitule_du_compte_bancaire->clearField();
    lineEdit_modifier_compte_bancaire_institut_bancaire->clearField();
    textEdit_modifier_compte_bancaire_description_du_compte->clear();
}
