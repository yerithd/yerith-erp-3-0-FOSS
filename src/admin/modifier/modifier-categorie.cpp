

void YerithAdminModifierWindow::setupEditCategorie()
{
    _windowName = QString("%1 - %2")
    				.arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
                		 QObject::tr("administration ~ modifier ~ catégories"));

    YerithAdminListerWindow *lw = _allWindows->_adminListerWindow;

    YerithSqlTableModel *categoriesTableModel = lw->getCurSearchSqlTableModel();

    if (0 == categoriesTableModel)
    {
        categoriesTableModel = &_allWindows->getSqlTableModel_categories();
    }
    else if (false ==
             YerithUtils::isEqualCaseInsensitive(categoriesTableModel->
                                                 sqlTableName(),
                                                 YerithDatabase::CATEGORIES))
    {
        categoriesTableModel = &_allWindows->getSqlTableModel_categories();
    }

    QSqlRecord record =
                    categoriesTableModel->record(lw->lastSelectedItemForModification());

    lineEdit_modifier_categorie_nom
		->setText(GET_SQL_RECORD_DATA(record,
                                      YerithDatabaseTableColumn::NOM_CATEGORIE));

    lineEdit_modifier_categorie_nom_departement_produit
		->setText(GET_SQL_RECORD_DATA(record,
                                      YerithDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT));

    textEdit_modifier_categorie_description->setText(GET_SQL_RECORD_DATA
                                                     (record,
                                                      YerithDatabaseTableColumn::DESCRIPTION_CATEGORIE));
}

void YerithAdminModifierWindow::modifier_categorie()
{
    if (modifier_categorie_check_fields())
    {
        YERITH_ERP_3_0_START_DATABASE_TRANSACTION;
        modifier_categorie_main();
        YERITH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;
    }
}

void YerithAdminModifierWindow::modifier_categorie_main()
{
    YerithAdminListerWindow *lw = _allWindows->_adminListerWindow;
    YerithSqlTableModel *categoriesTableModel =
                    lw->getCurSearchSqlTableModel();

    if (0 == categoriesTableModel)
    {
        categoriesTableModel = &_allWindows->getSqlTableModel_categories();
    }
    else if (!YerithUtils::isEqualCaseInsensitive
             (categoriesTableModel->sqlTableName(), YerithDatabase::CATEGORIES))
    {
        categoriesTableModel = &_allWindows->getSqlTableModel_categories();
    }

    QSqlRecord record =
                    categoriesTableModel->record(lw->lastSelectedItemForModification());

    QString
    oldNomCategorie(GET_SQL_RECORD_DATA
                    (record, YerithDatabaseTableColumn::NOM_CATEGORIE));

    QString newNomCategorie(lineEdit_modifier_categorie_nom->text());


    QSqlQuery CHECK_NomCategorie_QUERY;

    QString CHECK_WHETHER_nom_categorie_ALREADY_EXISTS_query =
    		QString("select %1 from %2 where %3='%4'")
				.arg(YerithDatabaseTableColumn::NOM_CATEGORIE,
					 YerithDatabase::CATEGORIES,
					 YerithDatabaseTableColumn::NOM_CATEGORIE,
					 newNomCategorie);

    int rowCount =
    		YerithUtils::execQuery(CHECK_NomCategorie_QUERY,
    							   CHECK_WHETHER_nom_categorie_ALREADY_EXISTS_query);

    if (rowCount > 0)
    {
    	QString retMsg_ALREADY_EXISTS(
    			QObject::tr("Une catégorie '%1' existe déjà !")
					.arg(newNomCategorie));

        YerithQMessageBox::warning(this,
                                   QObject::tr("Yerith-pgi-3.0 ~ admin-modifier-nom catégorie"),
								    retMsg_ALREADY_EXISTS);

        categoriesTableModel->resetFilter();

        return ;
    }

    record.setValue(YerithDatabaseTableColumn::NOM_CATEGORIE, newNomCategorie);
    record.setValue(YerithDatabaseTableColumn::DESCRIPTION_CATEGORIE,
                    textEdit_modifier_categorie_description->toPlainText());

    bool successCategorieTable =
                    categoriesTableModel->updateRecord(lw->
                                                       lastSelectedItemForModification(),
                                                       record);

    if (successCategorieTable)
    {
        //Handling of table "LIGNES_BUDGETAIRES"
        QString
        stocksQuery(QString("UPDATE %1 SET %2='%3' WHERE %4='%5'").arg
                    (YerithDatabase::LIGNES_BUDGETAIRES,
                     YerithDatabaseTableColumn::INTITULE_DE_LA_ligne_budgetaire,
					 newNomCategorie,
                     YerithDatabaseTableColumn::INTITULE_DE_LA_ligne_budgetaire,
					 oldNomCategorie));

        YerithUtils::execQuery(stocksQuery, 0);
    }

    if (successCategorieTable)
    {
        //Handling of table "stocks"
        QString
        stocksQuery(QString("UPDATE %1 SET %2='%3' WHERE %4='%5'").arg
                    (YerithDatabase::STOCKS,
                     YerithDatabaseTableColumn::CATEGORIE,
					 newNomCategorie,
                     YerithDatabaseTableColumn::CATEGORIE,
					 oldNomCategorie));

        YerithUtils::execQuery(stocksQuery, 0);
    }

    if (successCategorieTable)
    {
        ////Handling of table "stocks_vendu"
        QString
        stocksVenduQuery(QString("UPDATE %1 SET %2='%3' WHERE %4='%5'").arg
                         (YerithDatabase::STOCKS_VENDU,
                          YerithDatabaseTableColumn::CATEGORIE,
                          newNomCategorie,
                          YerithDatabaseTableColumn::CATEGORIE,
                          oldNomCategorie));

        YerithUtils::execQuery(stocksVenduQuery, 0);
    }

    if (successCategorieTable)
    {
        ////Handling of table "CHARGES_FINANCIERES"
        QString CHARGES_FINANCIERES_Query
			(QString("UPDATE %1 SET %2='%3' WHERE %4='%5'")
				.arg(YerithDatabase::CHARGES_FINANCIERES,
                     YerithDatabaseTableColumn::CATEGORIE,
                     newNomCategorie,
                     YerithDatabaseTableColumn::CATEGORIE,
                     oldNomCategorie));

        YerithUtils::execQuery(CHARGES_FINANCIERES_Query, 0);
    }

    if (successCategorieTable)
    {
        ////Handling of table "stocks_sorties"
        QString
		stocks_sorties_Query(QString("UPDATE %1 SET %2='%3' WHERE %4='%5'").arg
                         (YerithDatabase::STOCKS_SORTIES,
                          YerithDatabaseTableColumn::CATEGORIE,
                          newNomCategorie,
                          YerithDatabaseTableColumn::CATEGORIE,
                          oldNomCategorie));

        YerithUtils::execQuery(stocks_sorties_Query, 0);
    }

    if (successCategorieTable)
    {
        ////Handling of table "MARCHANDISES"
        QString
		MARCHANDISES_Query(QString("UPDATE %1 SET %2='%3' WHERE %4='%5'").arg
                         (YerithDatabase::MARCHANDISES,
                          YerithDatabaseTableColumn::CATEGORIE,
                          newNomCategorie,
                          YerithDatabaseTableColumn::CATEGORIE,
                          oldNomCategorie));

        YerithUtils::execQuery(MARCHANDISES_Query, 0);
    }

    QString retMsg(QObject::tr("Les données de la catégorie '%1'").arg
                   (lineEdit_modifier_categorie_nom->text()));

    if (successCategorieTable)
    {
        retMsg.
        append(QObject::tr(" ont été modifiées avec succès !"));

        YerithQMessageBox::information(this,
                                       QObject::tr
                                       ("Yerith-pgi-3.0 ~ admin-modifier-catégorie"),
                                       retMsg);

        _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_CATEGORIE);
        rendreInvisible();
    }
    else
    {
        qDebug() << "\t[reason for failing] " <<
                 categoriesTableModel->lastError();

        retMsg.append(QObject::tr(" n'ont pas pu être modifiées !"));

        YerithQMessageBox::warning(this,
                                   QObject::tr
                                   ("Yerith-pgi-3.0 ~ admin-modifier-catégorie"),
                                   retMsg);
    }
}

bool YerithAdminModifierWindow::modifier_categorie_check_fields()
{
    bool nom = lineEdit_modifier_categorie_nom->checkField();
    return nom;
}

void YerithAdminModifierWindow::clear_categorie_all_fields()
{
    lineEdit_modifier_categorie_nom->clearField();
    textEdit_modifier_categorie_description->clear();
}
