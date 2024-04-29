
bool YerithAdminCreateWindow::creer_localisation()
{
    if (creer_localisation_check_fields())
    {
        QString retMsg(QObject::tr("La localisation '%1'").arg
                       (lineEdit_creer_localisation_nom->text()));

        YerithSqlTableModel &localisationsTableModel =
                        _allWindows->getSqlTableModel_localisations();

        localisationsTableModel.yerithSetFilter_WITH_where_clause(QString
                                                                  ("LOWER(nom_localisation) = LOWER('%1')").
                                                                  arg
                                                                  (lineEdit_creer_localisation_nom->
                                                                   text()));

        //qDebug() << "++ filter: " << localisationsTableModel.filter();

        int localisationsTableModelRowCount =
                        localisationsTableModel.easySelect();

        if (localisationsTableModelRowCount > 0)
        {
            QSqlRecord record = localisationsTableModel.record(0);
            QString
            duplicateLocalisation(GET_SQL_RECORD_DATA
                                  (record,
                                   YerithDatabaseTableColumn::
                                   NOM_ENTREPRISE));

            //qDebug() << "++ duplicate localisation: " << duplicateLocalisation;

            retMsg.append(QObject::tr("%1' existe déjà !").arg
                          (duplicateLocalisation));

            YerithQMessageBox::warning(this,
                                       QObject::tr
                                       ("Yerith-pgi-3.0 ~ administration ~ créer ~ localisation"),
                                       retMsg);

            localisationsTableModel.resetFilter("src/admin/creer/creer-localisation.cpp");

            return false;
        }


        localisationsTableModel
			.resetFilter("src/admin/creer/creer-localisation.cpp",
        				 48);


        QSqlRecord record = localisationsTableModel.record();

        record.setValue(YerithDatabaseTableColumn::ID,
                        YerithERPWindows::getNextIdSqlTableModel_localisations());

        record.setValue("numero_unique",
                        lineEdit_creer_localisation_numero_unique->text());

        record.setValue("adresse_ip",
                        lineEdit_creer_localisation_adresse_ip->text());

        record.setValue("nom_localisation",
                        lineEdit_creer_localisation_nom->text());

        record.setValue(YerithDatabaseTableColumn::QUARTIER,
                        lineEdit_creer_localisation_quartier->text());

        record.setValue(YerithDatabaseTableColumn::VILLE,
                        lineEdit_creer_localisation_ville->text());

        record.setValue("province_etat",
                        lineEdit_creer_localisation_province_etat->text());

        record.setValue(YerithDatabaseTableColumn::PAYS,
                        lineEdit_creer_localisation_pays->text());

        record.setValue(YerithDatabaseTableColumn::BOITE_POSTALE,
                        lineEdit_creer_localisation_boite_postale->text());

        record.setValue("date_ouverture",
                        dateEdit_creer_localisation_date_ouverture->date());

        record.setValue(YerithDatabaseTableColumn::EMAIL,
                        lineEdit_creer_localisation_email->text());

        record.setValue(YerithDatabaseTableColumn::NUMERO_TELEPHONE_1,
                        lineEdit_creer_localisation_numero_telephone_1->text());

        record.setValue(YerithDatabaseTableColumn::NUMERO_TELEPHONE_2,
                        lineEdit_creer_localisation_numero_telephone_2->text());

        record.setValue("dbms",
                        comboBox_creer_localisation_base_donnees->currentText());

        record.setValue("description_lieu",
                        textEdit_creer_localisation_description_lieu->toPlainText());


        bool success =
        		localisationsTableModel
					.insertNewRecord(record,
									 _allWindows->_adminCreateWindow,
									 "src/admin/creer/creer-localisation.cpp",
									 101,
									 lineEdit_creer_localisation_nom->text());


        if (!success)
        {
            retMsg.append(QObject::tr("' n'a pas pu être créer !"));

            YerithQMessageBox::warning(this,
                                       QObject::tr
                                       ("Yerith-pgi-3.0 ~ administration ~ créer ~ localisation"),
                                       retMsg);
            return false;
        }

        retMsg.append(QObject::tr(" a été créer avec succès !"));

        YerithQMessageBox::information(this,
                                       QObject::tr
                                       ("Yerith-pgi-3.0 ~ administration ~ créer ~ localisation"),
                                       retMsg);

        clear_localisation_all_fields();

        _allWindows->_adminListerWindow->
        rendreVisible(SUJET_ACTION_LOCALISATION);
        rendreInvisible();
    }
    else
    {
        YerithQMessageBox::information(this,
                                       QObject::tr
                                       ("Yerith-pgi-3.0 ~ administration ~ créer ~ localisation"),
                                       QObject::tr
                                       ("BIEN VOULOIR REMPLIR TOUS LES "
                                        "CHAMPS DE TEXTES OBLIGATOIRES !"));

        return false;
    }

    return true;
}

bool YerithAdminCreateWindow::creer_localisation_check_fields()
{
    bool nom = lineEdit_creer_localisation_nom->checkField();

    return nom;
}

void YerithAdminCreateWindow::populateLocalisationComboBoxes()
{
    _logger->log("populateAlerteComboBoxes");

    comboBox_creer_localisation_base_donnees->populateComboBoxRawString
    (YerithDatabase::DBMS, QString("nom_dbms"));
}

void YerithAdminCreateWindow::clear_localisation_all_fields()
{
    lineEdit_creer_localisation_adresse_ip->clearField();
    lineEdit_creer_localisation_numero_unique->clearField();
    lineEdit_creer_localisation_nom->clearField();
    lineEdit_creer_localisation_quartier->clearField();
    lineEdit_creer_localisation_ville->clearField();
    lineEdit_creer_localisation_province_etat->clearField();
    lineEdit_creer_localisation_pays->clearField();
    lineEdit_creer_localisation_boite_postale->clearField();
    dateEdit_creer_localisation_date_ouverture->reset();
    lineEdit_creer_localisation_email->clearField();
    lineEdit_creer_localisation_numero_telephone_1->clearField();
    lineEdit_creer_localisation_numero_telephone_2->clearField();
    textEdit_creer_localisation_description_lieu->clear();
}
