

void YerithAdminModifierWindow::setupEditDepartementsDeProduits()
{
    _windowName = QString("%1 - %2")
    				.arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
                		 QObject::tr("administration ~ modifier ~ départements"));

    YerithAdminListerWindow *lw = _allWindows->_adminListerWindow;

    YerithSqlTableModel *departementsDeProduitsTableModel =
                    lw->getCurSearchSqlTableModel();

    if (0 == departementsDeProduitsTableModel)
    {
        departementsDeProduitsTableModel =
                        &_allWindows->getSqlTableModel_departements_produits();
    }
    else if (false ==
             YerithUtils::isEqualCaseInsensitive
             (departementsDeProduitsTableModel->sqlTableName(),
              YerithDatabase::DEPARTEMENTS_PRODUITS))
    {
        departementsDeProduitsTableModel =
                        &_allWindows->getSqlTableModel_departements_produits();
    }

    QSqlRecord record =
                    departementsDeProduitsTableModel->record(lw->
                                                             lastSelectedItemForModification
                                                             ());

    lineEdit_modifier_departements_de_produits_nom->setText(GET_SQL_RECORD_DATA
                                                            (record,
                                                             YerithDatabaseTableColumn::
                                                             NOM_DEPARTEMENT_PRODUIT));

    textEdit_modifier_departements_de_produits_description->setText
    (GET_SQL_RECORD_DATA
     (record, YerithDatabaseTableColumn::DESCRIPTION_DEPARTEMENT_PRODUIT));
}


void YerithAdminModifierWindow::modifier_departements_de_produits()
{
    if (modifier_departements_de_produits_check_fields())
    {
        YERITH_ERP_3_0_START_DATABASE_TRANSACTION;
        modifier_departements_de_produits_main();
        YERITH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;
    }
}


void YerithAdminModifierWindow::modifier_departements_de_produits_main()
{
    YerithAdminListerWindow *lw = _allWindows->_adminListerWindow;


    YerithSqlTableModel *departementsDeProduitsTableModel =
                    lw->getCurSearchSqlTableModel();


    if (0 == departementsDeProduitsTableModel)
    {
        departementsDeProduitsTableModel =
                        &_allWindows->getSqlTableModel_departements_produits();
    }
    else if (!YerithUtils::isEqualCaseInsensitive
             (departementsDeProduitsTableModel->sqlTableName(),
              YerithDatabase::DEPARTEMENTS_PRODUITS))
    {
        departementsDeProduitsTableModel =
                        &_allWindows->getSqlTableModel_departements_produits();
    }


    QSqlRecord record =
                    departementsDeProduitsTableModel->record(lw->
                                                             lastSelectedItemForModification
                                                             ());


    QString
    current_nom_departement_produit(GET_SQL_RECORD_DATA
                                    (record,
                                     YerithDatabaseTableColumn::
                                     NOM_DEPARTEMENT_PRODUIT));

    QString
    new_nom_departement_produit
    (lineEdit_modifier_departements_de_produits_nom->text());

    record.setValue(YerithDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
                    new_nom_departement_produit);

    record.setValue(YerithDatabaseTableColumn::DESCRIPTION_DEPARTEMENT_PRODUIT,
                    textEdit_modifier_departements_de_produits_description->
                    toPlainText());


    bool success_departements_produits_table =
                    departementsDeProduitsTableModel->updateRecord(lw->
                                                                   lastSelectedItemForModification
                                                                   (), record);


    if (success_departements_produits_table)
    {
        //Handling of table "stocks"
        QString
        stocksQuery(QString("UPDATE %1 SET %2='%3' WHERE %4='%5'").arg
                    (YerithDatabase::CATEGORIES,
                     YerithDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
                     new_nom_departement_produit,
                     YerithDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
                     current_nom_departement_produit));

        YerithUtils::execQuery(stocksQuery, 0);
    }

    if (success_departements_produits_table)
    {
        //Handling of table "stocks"
        QString
        stocksQuery(QString("UPDATE %1 SET %2='%3' WHERE %4='%5'").arg
                    (YerithDatabase::STOCKS,
                     YerithDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
                     new_nom_departement_produit,
                     YerithDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
                     current_nom_departement_produit));

        YerithUtils::execQuery(stocksQuery, 0);
    }

    if (success_departements_produits_table)
    {
        ////Handling of table "stocks_vendu"
        QString
        stocksVenduQuery(QString("UPDATE %1 SET %2='%3' WHERE %4='%5'").arg
                         (YerithDatabase::STOCKS_VENDU,
                          YerithDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
                          new_nom_departement_produit,
                          YerithDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
                          current_nom_departement_produit));

        YerithUtils::execQuery(stocksVenduQuery, 0);
    }

    if (success_departements_produits_table)
    {
        ////Handling of table "stocks_sorties"
        QString
        stocksVenduQuery(QString("UPDATE %1 SET %2='%3' WHERE %4='%5'").arg
                         (YerithDatabase::STOCKS_SORTIES,
                          YerithDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
                          new_nom_departement_produit,
                          YerithDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
                          current_nom_departement_produit));

        YerithUtils::execQuery(stocksVenduQuery, 0);
    }

    if (success_departements_produits_table)
    {
        ////Handling of table "inventaire_des_stocks"
        QString
        stocksVenduQuery(QString("UPDATE %1 SET %2='%3' WHERE %4='%5'").arg
                         (YerithDatabase::MARCHANDISES,
                          YerithDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
                          new_nom_departement_produit,
                          YerithDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
                          current_nom_departement_produit));

        YerithUtils::execQuery(stocksVenduQuery, 0);
    }

    QString
    retMsg(QObject::tr
           ("Les données du département de produits '%1'").arg
           (lineEdit_modifier_departements_de_produits_nom->text()));

    if (success_departements_produits_table)
    {
        retMsg.
        append(QObject::tr(" ont été modifiées avec succès !"));

        YerithQMessageBox::information(this,
                                       QObject::tr
                                       ("Yerith-pgi-3.0 ~ admin-modifier-département_de_produits"),
                                       retMsg);

        _allWindows->_adminListerWindow->rendreVisible
        (SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS);

        rendreInvisible();
    }
    else
    {
        qDebug() << "\t[reason for failing] " <<
                 departementsDeProduitsTableModel->lastError();

        retMsg.append(QObject::tr(" n'ont pas pu être modifiées !"));

        YerithQMessageBox::warning(this,
                                   QObject::tr
                                   ("Yerith-pgi-3.0 ~ admin-modifier-département_de_produits"),
                                   retMsg);
    }
}


void YerithAdminModifierWindow::clear_departements_de_produits_all_fields()
{
    lineEdit_modifier_departements_de_produits_nom->clearField();
    textEdit_modifier_departements_de_produits_description->clear();
}
