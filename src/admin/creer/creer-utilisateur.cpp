
bool YerithAdminCreateWindow::creer_utilisateur()
{
    //qDebug() << "YerithAdminCreateWindow::creer_utilisateur()";
    if (creer_utilisateur_check_fields())
    {
        QString retMsg(QObject::tr("Un compte utilisateur '"));

        YerithSqlTableModel &usersTableModel =
                        _allWindows->getSqlTableModel_users();

        QString nom_utilisateur(lineEdit_creer_utilisateur_id->text().
                                toLower());

        usersTableModel.yerithSetFilter_WITH_where_clause(QString
                                                          ("LOWER(nom_utilisateur) = LOWER('%1')").
                                                          arg
                                                          (nom_utilisateur));

        //qDebug() << "++ filter: " << usersTableModel.filter();

        int usersTableModelRowCount = usersTableModel.easySelect();

        //qDebug() << "++ result usersTableModelRowCount : " << usersTableModelRowCount ;

        if (usersTableModelRowCount > 0)
        {
            QSqlRecord record = usersTableModel.record(0);
            QString
            duplicateUtilisateur(GET_SQL_RECORD_DATA
                                 (record, "nom_utilisateur"));

            //qDebug() << "++ duplicate utilisateur: " << duplicateUtilisateur;

            retMsg.append(QObject::tr("%1' existe déjà !").arg
                          (duplicateUtilisateur));

            lineEdit_creer_utilisateur_id->
            setPalette(YerithUtils::YERITH_RED_PALETTE);

            YerithQMessageBox::warning(this,
                                       QObject::tr
                                       ("Yerith-pgi-3.0 ~ administration ~ créer ~ utilisateur"),
                                       retMsg);

            usersTableModel.resetFilter("src/admin/creer/creer-utilisateur.cpp");

            return false;
        }

        usersTableModel.resetFilter("src/admin/creer/creer-utilisateur.cpp");

        QString mot_passe(lineEdit_creer_utilisateur_mot_passe_1->text());

        if (YerithUtils::isEqualCaseInsensitive(mot_passe,
                                                lineEdit_creer_utilisateur_id->text
                                                ()))
        {
            QString msg(QObject::tr
                        ("Choisissez un mot de passe différent de votre id !"));

            YerithQMessageBox::warning(this,
                                       QObject::tr
                                       ("Yerith-pgi-3.0 ~ administration ~ créer ~ utilisateur"),
                                       msg, QMessageBox::Ok);
            return false;
        }

        QSqlRecord record = usersTableModel.record();

        record.setValue(YerithDatabaseTableColumn::ID,
                        YerithERPWindows::getNextIdSqlTableModel_users());
        record.setValue(YerithDatabaseTableColumn::PRENOM,
                        lineEdit_creer_utilisateur_prenom->text());
        record.setValue(YerithDatabaseTableColumn::NOM,
                        lineEdit_creer_utilisateur_nom->text());

        comboBox_creer_utilisateur_titre->saveCurrentValueToDatabase
        (YerithDatabaseTableColumn::TITRE, record);

        record.setValue(YerithDatabaseTableColumn::LIEU_NAISSANCE,
                        lineEdit_creer_utilisateur_lieu_naissance->text());
        record.setValue(YerithDatabaseTableColumn::DATE_NAISSANCE,
                        dateEdit_creer_utilisateur_date_naissance->date());
        record.setValue(YerithDatabaseTableColumn::EMAIL,
                        lineEdit_creer_utilisateur_email->text());
        record.setValue(YerithDatabaseTableColumn::PAYS,
                        lineEdit_creer_utilisateur_pays->text());
        record.setValue(YerithDatabaseTableColumn::VILLE,
                        lineEdit_creer_utilisateur_ville->text());
        record.setValue(YerithDatabaseTableColumn::PROVINCE_ETAT,
                        lineEdit_creer_utilisateur_province_etat->text());
        record.setValue(YerithDatabaseTableColumn::BOITE_POSTALE,
                        lineEdit_creer_utilisateur_boite_postale->text());
        record.setValue(YerithDatabaseTableColumn::NUMERO_TELEPHONE_1,
                        lineEdit_creer_utilisateur_numero_telephone_1->text
                        ());
        record.setValue(YerithDatabaseTableColumn::NUMERO_TELEPHONE_2,
                        lineEdit_creer_utilisateur_numero_telephone_2->text
                        ());

        comboBox_creer_utilisateur_role->saveCurrentValueToDatabase
        (YerithDatabaseTableColumn::ROLE, record);

        record.setValue(YerithDatabaseTableColumn::LOCALISATION,
                        YerithERPConfig::THIS_SITE_LOCALISATION_NAME);
        record.setValue(YerithDatabaseTableColumn::NOM_UTILISATEUR,
                        nom_utilisateur);

        QString nom_complet(lineEdit_creer_utilisateur_prenom->text());
        nom_complet.append(" ").append(lineEdit_creer_utilisateur_nom->
                                       text());

        record.setValue(YerithDatabaseTableColumn::NOM_COMPLET, nom_complet);

        mot_passe.append(nom_utilisateur);

        QByteArray md5Hash_mot_passe(MD5_HASH(mot_passe));

        record.setValue("mot_passe", md5Hash_mot_passe);

        retMsg.append(QObject::tr("%1' pour '%2 %3")
        				.arg(nom_utilisateur,
                             lineEdit_creer_utilisateur_prenom->text(),
                             lineEdit_creer_utilisateur_nom->text()));

        bool success =
        		usersTableModel
					.insertNewRecord(record,
									 _allWindows->_adminCreateWindow,
									 "src/admin/creer/creer-utilisateur.cpp",
									 127,
									 lineEdit_creer_utilisateur_nom->text());

        if (!success)
        {
            retMsg.append(QObject::tr("' n'a pas pu être créer!"));

            YerithQMessageBox::warning(this,
                                       QObject::tr
                                       ("Yerith-pgi-3.0 - créer"), retMsg);
            return false;
        }

        retMsg.append(QObject::tr("' a été créer !"));

        YerithQMessageBox::information(this,
                                       QObject::tr
                                       ("Yerith-pgi-3.0 - créer"), retMsg);

        clear_utilisateur_all_fields();

        _allWindows->_adminListerWindow->
        rendreVisible(SUJET_ACTION_COMPTE_UTILISATEUR);
        rendreInvisible();
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

    return true;
}

void YerithAdminCreateWindow::populateUtilisateurComboBoxes()
{
    _logger->log("populateUtilisateurComboBoxes");

    comboBox_creer_utilisateur_titre->
        setupPopulateNORawString(YerithDatabase::TITRES,
                                 YerithDatabaseTableColumn::APPELATION_TITRE,
                                 &YerithUtils::_titreToUserViewString);

    comboBox_creer_utilisateur_role->
        setupPopulateNORawString(YerithDatabase::ROLES,
                                 YerithDatabaseTableColumn::NOM_ROLE,
                                 &YerithUtils::_roleToUserViewString);

    comboBox_creer_utilisateur_titre->populateComboBox();

    comboBox_creer_utilisateur_role->populateComboBox();
}


void YerithAdminCreateWindow::creer_utilisateur_check_fields_entry()
{
    lineEdit_creer_utilisateur_nom->checkField();
    lineEdit_creer_utilisateur_prenom->checkField();
    comboBox_creer_utilisateur_titre->checkField();
    lineEdit_creer_utilisateur_id->checkField();
    comboBox_creer_utilisateur_role->checkField();

    bool mot_passe_1 = lineEdit_creer_utilisateur_mot_passe_1->checkField();
    bool verification_mot_de_passe =
                    lineEdit_creer_utilisateur_verification->checkField();

    //qDebug() << "++ verification_mot_de_passe" << BOOL_TO_STRING(verification_mot_de_passe);

    if (mot_passe_1 && verification_mot_de_passe)
    {
        if (lineEdit_creer_utilisateur_mot_passe_1->text() !=
                lineEdit_creer_utilisateur_verification->text())
        {
            lineEdit_creer_utilisateur_mot_passe_1->
            setPalette(YerithUtils::YERITH_RED_PALETTE);
            lineEdit_creer_utilisateur_verification->
            setPalette(YerithUtils::YERITH_RED_PALETTE);
        }
    }
}


bool YerithAdminCreateWindow::creer_utilisateur_check_fields()
{
    bool nom = lineEdit_creer_utilisateur_nom->checkField();
    bool prenom = lineEdit_creer_utilisateur_prenom->checkField();
    bool titre = comboBox_creer_utilisateur_titre->checkField();
    bool user_id = lineEdit_creer_utilisateur_id->checkField();
    bool mot_passe_1 = lineEdit_creer_utilisateur_mot_passe_1->checkField();
    bool verification_mot_de_passe =
                    lineEdit_creer_utilisateur_verification->checkField();
    bool role = comboBox_creer_utilisateur_role->checkField();

    //qDebug() << "++ verification_mot_de_passe" << BOOL_TO_STRING(verification_mot_de_passe);

    if (mot_passe_1 && verification_mot_de_passe)
    {
        if (lineEdit_creer_utilisateur_mot_passe_1->text() !=
                lineEdit_creer_utilisateur_verification->text())
        {
            QString
            msg(QObject::tr
                ("Les deux mot de passe sont différents !"));

            YerithQMessageBox::warning(this,
                                       QObject::tr
                                       ("Yerith-pgi-3.0 ~ administration ~ créer ~ utilisateur"),
                                       msg, QMessageBox::Ok);

            lineEdit_creer_utilisateur_mot_passe_1->
            setPalette(YerithUtils::YERITH_RED_PALETTE);
            lineEdit_creer_utilisateur_verification->
            setPalette(YerithUtils::YERITH_RED_PALETTE);

            return false;
        }
    }

    return nom &&
           prenom &&
           user_id && mot_passe_1 && verification_mot_de_passe && role && titre;
}

void YerithAdminCreateWindow::clear_utilisateur_all_fields()
{
    lineEdit_creer_utilisateur_nom->clearField();
    lineEdit_creer_utilisateur_prenom->clearField();
    lineEdit_creer_utilisateur_lieu_naissance->clearField();
    dateEdit_creer_utilisateur_date_naissance->reset();
    lineEdit_creer_utilisateur_numero_telephone_1->clearField();
    lineEdit_creer_utilisateur_numero_telephone_2->clearField();
    lineEdit_creer_utilisateur_id->clearField();
    lineEdit_creer_utilisateur_email->clearField();
    lineEdit_creer_utilisateur_ville->clearField();
    lineEdit_creer_utilisateur_province_etat->clearField();
    lineEdit_creer_utilisateur_pays->clearField();
    lineEdit_creer_utilisateur_boite_postale->clearField();
    lineEdit_creer_utilisateur_mot_passe_1->clearField();
    lineEdit_creer_utilisateur_verification->clearField();

    comboBox_creer_utilisateur_titre->clearField();
    comboBox_creer_utilisateur_role->clearField();
}
