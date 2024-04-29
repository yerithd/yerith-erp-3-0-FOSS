

void YerithAdminModifierWindow::setupEdit_LIGNE_BUDGETAIRE()
{
    _windowName = QString("%1 - %2")
    				.arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
                		 QObject::tr("administration ~ modifier ~ lignes budgétaires"));

    YerithAdminListerWindow *lw = _allWindows->_adminListerWindow;

    YerithSqlTableModel *LIGNES_BUDGETAIRES_TableModel =
                    lw->getCurSearchSqlTableModel();

    if (0 == LIGNES_BUDGETAIRES_TableModel)
    {
    	LIGNES_BUDGETAIRES_TableModel = &_allWindows->getSqlTableModel_lignes_budgetaires();
    }
    else if (false ==
             YerithUtils::isEqualCaseInsensitive(LIGNES_BUDGETAIRES_TableModel->sqlTableName(),
                                                 YerithDatabase::LIGNES_BUDGETAIRES))
    {
        LIGNES_BUDGETAIRES_TableModel = &_allWindows->getSqlTableModel_lignes_budgetaires();
    }

    QSqlRecord record =
                    LIGNES_BUDGETAIRES_TableModel->record(lw->lastSelectedItemForModification());

    lineEdit_MODIFIER_INTITULE_de_la_LIGNE_BUDGETAIRE
			->setText(GET_SQL_RECORD_DATA(record,
										  YerithDatabaseTableColumn::INTITULE_DE_LA_ligne_budgetaire));

    lineEdit_modifier_montant_de_la_LIGNE_BUDGETAIRE
    		->setText(GET_DOUBLE_STRING(
    				GET_SQL_RECORD_DATA(record,
    									YerithDatabaseTableColumn::montant_de_la_ligne_budgetaire).toDouble()));

    lineEdit_modifier_montant_RESTANT_de_la_LIGNE_BUDGETAIRE
    		->setText(GET_DOUBLE_STRING(
    				GET_SQL_RECORD_DATA(record,
    									YerithDatabaseTableColumn::montant_RESTANT_de_la_ligne_budgetaire).toDouble()));

    comboBox_modifier_INTITULE_DU_COMPTE_BANCAIRE
		->populateComboBoxRawString(YerithDatabase::COMPTES_BANCAIRES,
    								YerithDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE);

    comboBox_modifier_INTITULE_DU_COMPTE_BANCAIRE
		->setCurrentIndex(comboBox_modifier_INTITULE_DU_COMPTE_BANCAIRE
							->findText(GET_SQL_RECORD_DATA(record,
									   	   	   	   	   	   YerithDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE)));

    textEdit_modifier_une_LIGNE_budgetaire->setText(GET_SQL_RECORD_DATA
                                                     (record,
                                                      YerithDatabaseTableColumn::DESCRIPTION_ligne_budgetaire));

    lineEdit_MODIFIER_INTITULE_de_la_LIGNE_BUDGETAIRE->checkField();
    lineEdit_modifier_montant_de_la_LIGNE_BUDGETAIRE->checkField();
    comboBox_modifier_INTITULE_DU_COMPTE_BANCAIRE->checkField();
    textEdit_modifier_une_LIGNE_budgetaire->checkField();
}


void YerithAdminModifierWindow::modifier_LIGNE_BUDGETAIRE()
{
    if (modifier_LIGNE_BUDGETAIRE_check_fields())
    {
        YERITH_ERP_3_0_START_DATABASE_TRANSACTION;
        modifier_LIGNE_BUDGETAIRE_main();
        YERITH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;
    }
}


void YerithAdminModifierWindow::modifier_LIGNE_BUDGETAIRE_main()
{
    YerithAdminListerWindow *lw = _allWindows->_adminListerWindow;

    YerithSqlTableModel *LIGNES_BUDGETAIRES_TableModel =
                    lw->getCurSearchSqlTableModel();

    if (0 == LIGNES_BUDGETAIRES_TableModel)
    {
        LIGNES_BUDGETAIRES_TableModel = &_allWindows->getSqlTableModel_lignes_budgetaires();
    }
    else if (!YerithUtils::isEqualCaseInsensitive
             (LIGNES_BUDGETAIRES_TableModel->sqlTableName(), YerithDatabase::LIGNES_BUDGETAIRES))
    {
        LIGNES_BUDGETAIRES_TableModel = &_allWindows->getSqlTableModel_lignes_budgetaires();
    }

    QSqlRecord record =
                    LIGNES_BUDGETAIRES_TableModel->record(lw->lastSelectedItemForModification());


    QString ANCIEN_INTITULE_de_la_LIGNE_BUDEGTAIRE
				(GET_SQL_RECORD_DATA(record,
									 YerithDatabaseTableColumn::INTITULE_DE_LA_ligne_budgetaire));

    QString nouveau_INTITULE_de_la_LIGNE_BUDEGTAIRE(
    			lineEdit_MODIFIER_INTITULE_de_la_LIGNE_BUDGETAIRE->text());


    QSqlQuery CHECK_lignes_budgetaires_QUERY;

    QString CHECK_WHETHER_intitule_de_la_ligne_budgetaire_ALREADY_EXISTS_query =
    		QString("select %1 from %2 where %3='%4';")
				.arg(YerithDatabaseTableColumn::INTITULE_DE_LA_ligne_budgetaire,
					 YerithDatabase::LIGNES_BUDGETAIRES,
					 YerithDatabaseTableColumn::INTITULE_DE_LA_ligne_budgetaire,
					 nouveau_INTITULE_de_la_LIGNE_BUDEGTAIRE);

    int rowCount =
    		YerithUtils::execQuery(CHECK_lignes_budgetaires_QUERY,
    							   CHECK_WHETHER_intitule_de_la_ligne_budgetaire_ALREADY_EXISTS_query);

    if (rowCount > 1)
    {
    	QString retMsg_ALREADY_EXISTS(
    			QObject::tr("UNE LIGNE BUDGÉTAIRE '%1' existe déjà !")
					.arg(nouveau_INTITULE_de_la_LIGNE_BUDEGTAIRE));

        YerithQMessageBox::warning(this,
                                   QObject::tr("Yerith-pgi-3.0 ~ admin-modifier-ligne budgétaire"),
								    retMsg_ALREADY_EXISTS);

        LIGNES_BUDGETAIRES_TableModel->resetFilter();

        return ;
    }

    record.setValue(YerithDatabaseTableColumn::INTITULE_DE_LA_ligne_budgetaire,
    				nouveau_INTITULE_de_la_LIGNE_BUDEGTAIRE);

    record.setValue(YerithDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE,
    				comboBox_modifier_INTITULE_DU_COMPTE_BANCAIRE->currentText());

    record.setValue(YerithDatabaseTableColumn::DESCRIPTION_ligne_budgetaire,
    				textEdit_modifier_une_LIGNE_budgetaire->toPlainText());

    bool success_INTITULE_DE_LA_ligne_budgetaire_Table =
                    LIGNES_BUDGETAIRES_TableModel
						->updateRecord(lw->lastSelectedItemForModification(),
                                       record);

    /*
     * THE MODIFICATION OF A BUDGET LINE TITLE
     * is equivalent to the MODIFICATION OF
     * A PRODUCT CATEGORY !!!
     */

    if (success_INTITULE_DE_LA_ligne_budgetaire_Table)
      {
          //Handling of table "stocks"
          QString
          stocksQuery(QString("UPDATE %1 SET %2='%3' WHERE %4='%5'").arg
                      (YerithDatabase::STOCKS,
                       YerithDatabaseTableColumn::CATEGORIE,
					   nouveau_INTITULE_de_la_LIGNE_BUDEGTAIRE,
                       YerithDatabaseTableColumn::CATEGORIE,
					   ANCIEN_INTITULE_de_la_LIGNE_BUDEGTAIRE));

          YerithUtils::execQuery(stocksQuery, 0);
      }

      if (success_INTITULE_DE_LA_ligne_budgetaire_Table)
      {
          ////Handling of table "stocks_vendu"
          QString
          stocksVenduQuery(QString("UPDATE %1 SET %2='%3' WHERE %4='%5'").arg
                           (YerithDatabase::STOCKS_VENDU,
                            YerithDatabaseTableColumn::CATEGORIE,
							nouveau_INTITULE_de_la_LIGNE_BUDEGTAIRE,
                            YerithDatabaseTableColumn::CATEGORIE,
							ANCIEN_INTITULE_de_la_LIGNE_BUDEGTAIRE));

          YerithUtils::execQuery(stocksVenduQuery, 0);
      }

      if (success_INTITULE_DE_LA_ligne_budgetaire_Table)
      {
          ////Handling of table "CHARGES_FINANCIERES"
          QString CHARGES_FINANCIERES_Query
  			(QString("UPDATE %1 SET %2='%3' WHERE %4='%5'")
  				.arg(YerithDatabase::CHARGES_FINANCIERES,
                       YerithDatabaseTableColumn::CATEGORIE,
					   nouveau_INTITULE_de_la_LIGNE_BUDEGTAIRE,
                       YerithDatabaseTableColumn::CATEGORIE,
					   ANCIEN_INTITULE_de_la_LIGNE_BUDEGTAIRE));

          YerithUtils::execQuery(CHARGES_FINANCIERES_Query, 0);
      }

      if (success_INTITULE_DE_LA_ligne_budgetaire_Table)
      {
          ////Handling of table "stocks_sorties"
          QString
  		stocks_sorties_Query(QString("UPDATE %1 SET %2='%3' WHERE %4='%5'").arg
                           (YerithDatabase::STOCKS_SORTIES,
                            YerithDatabaseTableColumn::CATEGORIE,
							nouveau_INTITULE_de_la_LIGNE_BUDEGTAIRE,
                            YerithDatabaseTableColumn::CATEGORIE,
							ANCIEN_INTITULE_de_la_LIGNE_BUDEGTAIRE));

          YerithUtils::execQuery(stocks_sorties_Query, 0);
      }

      if (success_INTITULE_DE_LA_ligne_budgetaire_Table)
      {
          ////Handling of table "CATEGORIES"
          QString
  		MARCHANDISES_Query(QString("UPDATE %1 SET %2='%3' WHERE %4='%5'").arg
                           (YerithDatabase::CATEGORIES,
                            YerithDatabaseTableColumn::NOM_CATEGORIE,
							nouveau_INTITULE_de_la_LIGNE_BUDEGTAIRE,
                            YerithDatabaseTableColumn::NOM_CATEGORIE,
							ANCIEN_INTITULE_de_la_LIGNE_BUDEGTAIRE));

          YerithUtils::execQuery(MARCHANDISES_Query, 0);
      }

      if (success_INTITULE_DE_LA_ligne_budgetaire_Table)
      {
          ////Handling of table "MARCHANDISES"
          QString
  		MARCHANDISES_Query(QString("UPDATE %1 SET %2='%3' WHERE %4='%5'").arg
                           (YerithDatabase::MARCHANDISES,
                            YerithDatabaseTableColumn::CATEGORIE,
							nouveau_INTITULE_de_la_LIGNE_BUDEGTAIRE,
                            YerithDatabaseTableColumn::CATEGORIE,
							ANCIEN_INTITULE_de_la_LIGNE_BUDEGTAIRE));

          YerithUtils::execQuery(MARCHANDISES_Query, 0);
      }

    QString retMsg(QObject::tr("Les données de la LIGNE BUDGÉTAIRE '%1'")
    					.arg(lineEdit_MODIFIER_INTITULE_de_la_LIGNE_BUDGETAIRE->text()));

    if (success_INTITULE_DE_LA_ligne_budgetaire_Table)
    {
        retMsg.append(QObject::tr(" ont été modifiées avec succès !"));

        YerithQMessageBox::information(this,
                                       QObject::tr("Yerith-pgi-3.0 ~ admin-modifier-ligne budgétaire"),
                                       retMsg);

        _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_ligne_budgetaire);
        rendreInvisible();
    }
    else
    {
        qDebug() << "\t[reason for failing] " <<
                 LIGNES_BUDGETAIRES_TableModel->lastError();

        retMsg.append(QObject::tr(" n'ont pas pu être modifiées !"));

        YerithQMessageBox::warning(this,
                                   QObject::tr("Yerith-pgi-3.0 ~ admin-modifier-ligne budgétaire"),
                                   retMsg);
    }
}


bool YerithAdminModifierWindow::modifier_LIGNE_BUDGETAIRE_check_fields()
{
    bool intitule_de_la_ligne_budgetaire = lineEdit_MODIFIER_INTITULE_de_la_LIGNE_BUDGETAIRE->checkField();

    bool INTITULE_DU_COMPTE_BANCAIRE = comboBox_modifier_INTITULE_DU_COMPTE_BANCAIRE->checkField();

    return INTITULE_DU_COMPTE_BANCAIRE    &&
    	   intitule_de_la_ligne_budgetaire;
}


void YerithAdminModifierWindow::clear_LIGNE_BUDGETAIRE_all_fields()
{
	lineEdit_MODIFIER_INTITULE_de_la_LIGNE_BUDGETAIRE->clearField();
	lineEdit_modifier_montant_de_la_LIGNE_BUDGETAIRE->clearField();
	lineEdit_modifier_montant_RESTANT_de_la_LIGNE_BUDGETAIRE->clearField();
	comboBox_modifier_INTITULE_DU_COMPTE_BANCAIRE->clearField();
	textEdit_modifier_une_LIGNE_budgetaire->clear();
}
