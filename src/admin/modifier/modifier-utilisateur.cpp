
void YerithAdminModifierWindow::setupEditCompteUtilisateur()
{
    _windowName = QString("%1 - %2")
    				.arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
                		 QObject::tr("administration ~ modifier ~ utilisateurs"));

    YerithAdminListerWindow *lw = _allWindows->_adminListerWindow;

    YerithSqlTableModel *userTableModel = lw->getCurSearchSqlTableModel();

    if (!userTableModel)
    {
        userTableModel = &_allWindows->getSqlTableModel_users();
    }
    else if (userTableModel &&
             !YerithUtils::isEqualCaseInsensitive(userTableModel->
                                                  sqlTableName(),
                                                  YerithDatabase::USERS))
    {
        userTableModel = &_allWindows->getSqlTableModel_users();
    }

    QSqlRecord record =
                    userTableModel->record(lw->lastSelectedItemForModification());

    lineEdit_modifier_utilisateur_id->setText(GET_SQL_RECORD_DATA
                                              (record,
                                               YerithDatabaseTableColumn::NOM_UTILISATEUR));
    lineEdit_modifier_utilisateur_mot_passe_1->setText("#####");
    lineEdit_modifier_utilisateur_mot_passe_2->setText("#####");
    lineEdit_modifier_utilisateur_prenom->setText(GET_SQL_RECORD_DATA
                                                  (record,
                                                   YerithDatabaseTableColumn::PRENOM));
    lineEdit_modifier_utilisateur_nom->setText(GET_SQL_RECORD_DATA
                                               (record,
                                                YerithDatabaseTableColumn::NOM));

    QString
    date_naissance(GET_SQL_RECORD_DATA
                   (record, YerithDatabaseTableColumn::DATE_NAISSANCE));
    dateEdit_modifier_utilisateur_date_naissance->setDate(GET_DATE_FROM_STRING
                                                          (date_naissance));

    lineEdit_modifier_utilisateur_lieu_naissance->setText(GET_SQL_RECORD_DATA
                                                          (record,
                                                           YerithDatabaseTableColumn::LIEU_NAISSANCE));
    lineEdit_modifier_utilisateur_ville->setText(GET_SQL_RECORD_DATA
                                                 (record,
                                                  YerithDatabaseTableColumn::VILLE));
    lineEdit_modifier_utilisateur_province_etat->setText(GET_SQL_RECORD_DATA
                                                         (record,
                                                          YerithDatabaseTableColumn::PROVINCE_ETAT));
    lineEdit_modifier_utilisateur_pays->setText(GET_SQL_RECORD_DATA
                                                (record,
                                                 YerithDatabaseTableColumn::PAYS));
    lineEdit_modifier_utilisateur_boite_postale->setText(GET_SQL_RECORD_DATA
                                                         (record,
                                                          YerithDatabaseTableColumn::BOITE_POSTALE));
    lineEdit_modifier_utilisateur_email->setText(GET_SQL_RECORD_DATA
                                                 (record,
                                                  YerithDatabaseTableColumn::EMAIL));
    lineEdit_modifier_utilisateur_numero_telephone_1->setText
    (GET_SQL_RECORD_DATA
     (record, YerithDatabaseTableColumn::NUMERO_TELEPHONE_1));
    lineEdit_modifier_utilisateur_numero_telephone_2->setText
    (GET_SQL_RECORD_DATA
     (record, YerithDatabaseTableColumn::NUMERO_TELEPHONE_2));

    int a_temp_code = -1;

    {
        a_temp_code =
                        GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::TITRE).toInt();

        comboBox_modifier_utilisateur_titre->populateComboBoxMissing
        (a_temp_code);

        comboBox_modifier_utilisateur_titre->setCurrentIndex
        (comboBox_modifier_utilisateur_titre->findText
         (YerithUtils::_titreToUserViewString.value(a_temp_code)));
    }

    {
        a_temp_code =
                        GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::ROLE).toInt();

        comboBox_modifier_utilisateur_role->populateComboBoxMissing(a_temp_code);

        comboBox_modifier_utilisateur_role->setCurrentIndex
        (comboBox_modifier_utilisateur_role->findText
         (YerithUtils::_roleToUserViewString.value(a_temp_code)));
    }

    lineEdit_modifier_utilisateur_localisation->setText(YerithERPConfig::
                                                        THIS_SITE_LOCALISATION_NAME);
}

void YerithAdminModifierWindow::modifier_utilisateur()
{
    if (modifier_utilisateur_check_fields())
    {
        YERITH_ERP_3_0_START_DATABASE_TRANSACTION;
        modifier_utilisateur_main();
        YERITH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;
    }
}

void YerithAdminModifierWindow::modifier_utilisateur_main()
{
    if (modifier_utilisateur_check_fields())
    {
        YerithAdminListerWindow *lw = _allWindows->_adminListerWindow;
        YerithSqlTableModel *userTableModel = lw->getCurSearchSqlTableModel();

        if (!userTableModel)
        {
            userTableModel = &_allWindows->getSqlTableModel_users();
        }
        else if (userTableModel &&
                 !YerithUtils::isEqualCaseInsensitive(userTableModel->
                                                      sqlTableName(),
                                                      YerithDatabase::USERS))
        {
            userTableModel = &_allWindows->getSqlTableModel_users();
        }

        QSqlRecord record =
                        userTableModel->record(lw->lastSelectedItemForModification());

        record.setValue("prenom",
                        lineEdit_modifier_utilisateur_prenom->text());
        record.setValue("nom", lineEdit_modifier_utilisateur_nom->text());

        comboBox_modifier_utilisateur_titre->saveCurrentValueToDatabase
        (YerithDatabaseTableColumn::TITRE, record);

        record.setValue(YerithDatabaseTableColumn::LIEU_NAISSANCE,
                        lineEdit_modifier_utilisateur_lieu_naissance->text());
        record.setValue(YerithDatabaseTableColumn::DATE_NAISSANCE,
                        dateEdit_modifier_utilisateur_date_naissance->date());
        record.setValue(YerithDatabaseTableColumn::VILLE,
                        lineEdit_modifier_utilisateur_ville->text());
        record.setValue(YerithDatabaseTableColumn::PROVINCE_ETAT,
                        lineEdit_modifier_utilisateur_province_etat->text());
        record.setValue(YerithDatabaseTableColumn::PAYS,
                        lineEdit_modifier_utilisateur_pays->text());
        record.setValue(YerithDatabaseTableColumn::BOITE_POSTALE,
                        lineEdit_modifier_utilisateur_boite_postale->text());
        record.setValue(YerithDatabaseTableColumn::EMAIL,
                        lineEdit_modifier_utilisateur_email->text());
        record.setValue(YerithDatabaseTableColumn::NUMERO_TELEPHONE_1,
                        lineEdit_modifier_utilisateur_numero_telephone_1->text
                        ());
        record.setValue(YerithDatabaseTableColumn::NUMERO_TELEPHONE_2,
                        lineEdit_modifier_utilisateur_numero_telephone_2->text
                        ());

        comboBox_modifier_utilisateur_role->saveCurrentValueToDatabase
        (YerithDatabaseTableColumn::ROLE, record);

        record.setValue("localisation",
                        lineEdit_modifier_utilisateur_localisation->text());

        QString nom_complet(lineEdit_modifier_utilisateur_prenom->text());
        nom_complet.append(" ").append(lineEdit_modifier_utilisateur_nom->
                                       text());

        record.setValue("nom_complet", nom_complet);

        if (_savePassword)
        {
            QString
            mot_passe_1(lineEdit_modifier_utilisateur_mot_passe_1->text());

            mot_passe_1.append(lineEdit_modifier_utilisateur_id->text().
                               toLower());

            QByteArray md5Hash_mot_passe(MD5_HASH(mot_passe_1));

            //qDebug() << "++ modifier_utilisateur: " << md5Hash_mot_passe;
            record.setValue("mot_passe", md5Hash_mot_passe);
        }

        bool success =
                        userTableModel->updateRecord(lw->lastSelectedItemForModification(),
                                                     record);

        QString retMsg(QObject::tr("Les données de l'utilisateur '%1'").
                       arg(lineEdit_modifier_utilisateur_id->text()));

        if (success)
        {
            retMsg.
            append(QObject::tr(" ont été modifiées avec succès !"));

            YerithQMessageBox::information(this,
                                           QObject::tr
                                           ("Yerith-pgi-3.0 ~ admin-modifier-utilisateur"),
                                           retMsg);

            _allWindows->_adminListerWindow->rendreVisible
            (SUJET_ACTION_COMPTE_UTILISATEUR);
            rendreInvisible();
        }
        else
        {
            qDebug() <<
                     QString("\t[reason for failing] %1").
                     arg(userTableModel->lastError().text());

            retMsg.
            append(QObject::tr(" n'ont pas pu être modifiées !"));

            YerithQMessageBox::warning(this,
                                       QObject::tr
                                       ("Yerith-pgi-3.0 ~ admin-modifier-utilisateur"),
                                       retMsg);
        }
    }
}

bool YerithAdminModifierWindow::modifier_utilisateur_check_fields()
{
    QString mot_passe_1(lineEdit_modifier_utilisateur_mot_passe_1->text());
    QString mot_passe_2(lineEdit_modifier_utilisateur_mot_passe_2->text());

    if (mot_passe_1 != "#####" || mot_passe_2 != "#####")
    {
        if (mot_passe_1 != mot_passe_2)
        {
            QString
            msg(QObject::tr
                ("Les deux mot de passe sont différents !"));

            YerithQMessageBox::warning(this,
                                       QObject::tr
                                       ("Yerith-pgi-3.0 -  administration ~ modifier ~ utilisateur"),
                                       msg, QMessageBox::Ok);

            lineEdit_modifier_utilisateur_mot_passe_1->
            setPalette(YerithUtils::YERITH_RED_PALETTE);
            lineEdit_modifier_utilisateur_mot_passe_2->
            setPalette(YerithUtils::YERITH_RED_PALETTE);

            _savePassword = false;

            return false;
        }
        else
        {
            _savePassword = true;
        }
    }

    bool nom = lineEdit_modifier_utilisateur_nom->checkField();
    bool prenom = lineEdit_modifier_utilisateur_prenom->checkField();
    bool role = comboBox_modifier_utilisateur_role->checkField();

    return nom && prenom && role;
}

void YerithAdminModifierWindow::clear_utilisateur_all_fields()
{
    lineEdit_modifier_utilisateur_nom->clearField();
    lineEdit_modifier_utilisateur_prenom->clearField();
    lineEdit_modifier_utilisateur_lieu_naissance->clearField();
    lineEdit_modifier_utilisateur_numero_telephone_1->clearField();
    lineEdit_modifier_utilisateur_numero_telephone_2->clearField();
    lineEdit_modifier_utilisateur_id->clearField();
    lineEdit_modifier_utilisateur_ville->clearField();
    lineEdit_modifier_utilisateur_province_etat->clearField();
    lineEdit_modifier_utilisateur_pays->clearField();
    lineEdit_modifier_utilisateur_boite_postale->clearField();
    lineEdit_modifier_utilisateur_email->clearField();
    lineEdit_modifier_utilisateur_mot_passe_1->clearField();
    lineEdit_modifier_utilisateur_mot_passe_2->clearField();
    lineEdit_modifier_utilisateur_localisation->clearField();

    comboBox_modifier_utilisateur_titre->clear();
    comboBox_modifier_utilisateur_role->clear();
}
