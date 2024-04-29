
bool YerithAdminCreateWindow::creer_departements_de_produits()
{
    if (creer_departements_de_produits_check_fields())
    {
        QString retMsg(QObject::tr("Le département '"));

        YerithSqlTableModel &departements_produitsTableModel =
                        _allWindows->getSqlTableModel_departements_produits();

        departements_produitsTableModel.yerithSetFilter_WITH_where_clause
        (QString("LOWER(%1) = LOWER('%2')").arg
         (YerithDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
          lineEdit_creer_departements_de_produits_nom->text()));

        //qDebug() << "++ filter: " << departements_produitsTableModel.filter();

        int departements_produitsTableModelRowCount =
        		departements_produitsTableModel.easySelect("src/admin/creer/creer-departements-de-produits.cpp",
								 	 	 	 	 	 	 	18);

        //qDebug() << "++ result departements_produitsTableModelRowCount : " << departements_produitsTableModelRowCount ;

        if (departements_produitsTableModelRowCount > 0)
        {
            QSqlRecord record = departements_produitsTableModel.record(0);

            QString duplicateDepartement_de_produits
					(GET_SQL_RECORD_DATA(record,
                                         YerithDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT));

            //qDebug() << "++ duplicate departement_de_produits: " << duplicateDepartement_de_produits;

            retMsg.append(QObject::tr("%1' existe déjà ! ")
            				.arg(duplicateDepartement_de_produits));

            YerithQMessageBox::warning(this,
                                       QObject::tr
                                       ("Yerith-pgi-3.0 ~ administration ~ créer ~ département de produits"),
                                       retMsg);

            departements_produitsTableModel
				.resetFilter("src/admin/creer/creer-departements-de-produits.cpp",
							 43);

            return false;
        }

        departements_produitsTableModel
			.resetFilter("src/admin/creer/creer-departements-de-produits.cpp",
						 50);

        QSqlRecord record = departements_produitsTableModel.record();

        record.setValue(YerithDatabaseTableColumn::ID,
                        YerithERPWindows::getNextIdSqlTableModel_departements_produits());

        record.setValue(YerithDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
                        lineEdit_creer_departements_de_produits_nom->text());

        record.setValue(YerithDatabaseTableColumn::DESCRIPTION_DEPARTEMENT_PRODUIT,
                 	 	textEdit_creer_departements_de_produits_description->toPlainText());

        bool success =
        		departements_produitsTableModel
					.insertNewRecord(record,
									 _allWindows->_adminCreateWindow,
									 "src/admin/creer/creer-departements-de-produits.cpp",
									 64,
									 lineEdit_creer_departements_de_produits_nom->text());

        if (!success)
        {
            retMsg.append(QObject::tr("%1' n'a pas pu être créer ! ")
            				 .arg(lineEdit_creer_departements_de_produits_nom->text()));

            YerithQMessageBox::warning(this,
                                       QObject::tr("Yerith-pgi-3.0 ~ admin-créer-département de produits"),
                                       retMsg);
            return false;
        }


        retMsg.append(QObject::tr("%1' a été créer avec succès ! ")
                       .arg(lineEdit_creer_departements_de_produits_nom->text()));


        YerithQMessageBox::information(this,
                                       QObject::tr("Yerith-pgi-3.0 ~ admin-créer-département de produits"),
                                       retMsg);

        clear_departements_de_produits_all_fields();

        rendreInvisible();

        _allWindows->_adminListerWindow
			->rendreVisible(SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS);

        return true;
    }
    else
    {
        YerithQMessageBox::information(this,
                                       QObject::tr("Yerith-pgi-3.0 ~ admin-créer-département de produits"),
                                       QObject::tr("BIEN VOULOIR REMPLIR TOUS LES "
                                    		   	   "CHAMPS DE TEXTES OBLIGATOIRES !"));

        return false;
    }

    return false;
}

bool YerithAdminCreateWindow::creer_departements_de_produits_check_fields()
{
    bool nom = lineEdit_creer_departements_de_produits_nom->checkField();
    return nom;
}

void YerithAdminCreateWindow::clear_departements_de_produits_all_fields()
{
    lineEdit_creer_departements_de_produits_nom->clearField();
    textEdit_creer_departements_de_produits_description->clear();
}
