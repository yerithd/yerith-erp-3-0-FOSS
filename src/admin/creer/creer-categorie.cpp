
bool YerithAdminCreateWindow::creer_categorie()
{
    if (creer_categorie_check_fields())
    {
        QString retMsg(QObject::tr("La catégorie '"));

        YerithSqlTableModel &categoriesTableModel =
                        _allWindows->getSqlTableModel_categories();

        categoriesTableModel.yerithSetFilter_WITH_where_clause(QString
                                                               ("LOWER(nom_categorie) = LOWER('%1')").
                                                               arg
                                                               (lineEdit_creer_categorie_nom->
                                                                text()));

        //qDebug() << "++ filter: " << categoriesTableModel.filter();

        int categoriesTableModelRowCount = categoriesTableModel.easySelect();

        //qDebug() << "++ result categoriesTableModelRowCount : " << categoriesTableModelRowCount ;

        if (categoriesTableModelRowCount > 0)
        {
            QSqlRecord record = categoriesTableModel.record(0);

            QString duplicateCategorie
						(GET_SQL_RECORD_DATA(record,
                                			 YerithDatabaseTableColumn::NOM_CATEGORIE));

            //qDebug() << "++ duplicate categorie: " << duplicateCategorie;

            retMsg.append(QObject::tr("%1' existe déjà ! ")
                          	  .arg(duplicateCategorie));


            YerithQMessageBox::warning(this,
                                       QObject::tr("catégorie"),
									   retMsg);


            categoriesTableModel
				.resetFilter("src/admin/creer/creer-categorie.cpp",
							 42);


            return false;
        }


        categoriesTableModel
			.resetFilter("src/admin/creer/creer-categorie.cpp",
						 51);


        QSqlRecord record = categoriesTableModel.record();

        record.setValue(YerithDatabaseTableColumn::ID,
                        YerithERPWindows::getNextIdSqlTableModel_categories());

        record.setValue(YerithDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
                        comboBox_nom_departement_produit->currentText());

        record.setValue(YerithDatabaseTableColumn::NOM_CATEGORIE,
                        lineEdit_creer_categorie_nom->text());

        record.setValue(YerithDatabaseTableColumn::DESCRIPTION_CATEGORIE,
                        textEdit_creer_categorie_description->toPlainText());


        bool success = categoriesTableModel
        				.insertNewRecord(record,
        								 _allWindows->_adminCreateWindow,
										 "src/admin/creer/creer-categorie.cpp",
										 71,
										 lineEdit_creer_categorie_nom->text());


        if (!success)
        {
            retMsg.append(QObject::tr("%1' n'a pas pu être créer ! ")
                          	  .arg(lineEdit_creer_categorie_nom->text()));

            YerithQMessageBox::warning(this,
                                       QObject::tr
                                       ("Yerith-pgi-3.0 ~ admin-créer-catégorie"),
                                       retMsg);
            return false;
        }

        retMsg.append(QObject::tr("%1' a été créer avec succès ! ").
                      arg(lineEdit_creer_categorie_nom->text()));

        YerithQMessageBox::information(this,
                                       QObject::tr("Yerith-pgi-3.0 ~ admin-créer-catégorie"),
                                       retMsg);

        clear_categorie_all_fields();

        rendreInvisible();

        _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_CATEGORIE);

        return true;
    }
    else
    {
        YerithQMessageBox::information(this,
                                       QObject::tr
                                       ("Yerith-pgi-3.0 ~ admin-créer-catégorie"),
                                       QObject::tr
                                       ("BIEN VOULOIR REMPLIR TOUS LES "
                                        "CHAMPS DE TEXTES OBLIGATOIRES !"));

        return false;
    }

    return false;
}

bool YerithAdminCreateWindow::creer_categorie_check_fields()
{
    bool nom_categorie = lineEdit_creer_categorie_nom->checkField();
    bool nom_departement_produit =
                    comboBox_nom_departement_produit->checkField();

    return nom_categorie && nom_departement_produit;
}

void YerithAdminCreateWindow::clear_categorie_all_fields()
{
    lineEdit_creer_categorie_nom->clearField();
    textEdit_creer_categorie_description->clear();
}
