/*
 * yerith-erp-database-table-column.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-database-table-column.hpp"

#include "src/include/yerith-erp-3-0-software.text-configuration.hpp"

#include "src/utils/yerith-erp-utils.hpp"


QMap<QString, QString> YerithDatabaseTableColumn::_tableColumnToUserViewString;


const QString YerithDatabaseTableColumn::IS_SERVICE("is_service");

const QString YerithDatabaseTableColumn::IS_VENTE_EN_GROS("is_vente_en_gros");

const QString YerithDatabaseTableColumn::ID_ALERTE("id_alerte");

const QString YerithDatabaseTableColumn::ID("id");

const QString YerithDatabaseTableColumn::ERROR_ACCEPTED_RUNTIME_MONITOR_NAME("error_accepted_runtime_monitor_NAME");

const QString YerithDatabaseTableColumn::ERROR_ACCEPTED_RUNTIME_MONITOR_NAME_ID("error_accepted_runtime_monitor_name_ID");

const QString YerithDatabaseTableColumn::ERROR_ACCEPTED_RUNTIME_MONITOR_NAME_DESCRIPTION("error_accepted_runtime_monitor_name_DESCRIPTION");

const QString YerithDatabaseTableColumn::REFEREUR_CLIENT("refereur_client");

const QString YerithDatabaseTableColumn::REFEREUR_CLIENT_ID("refereur_client_ID");

const QString YerithDatabaseTableColumn::GROUPES_DU_CLIENT("groupes_du_client");

const QString YerithDatabaseTableColumn::GROUPES_DU_CLIENT_ID("groupes_du_client_ID");

const QString YerithDatabaseTableColumn::GROUPE_DE_PAIE_HR("groupe_de_paie_hr");

const QString YerithDatabaseTableColumn::GROUPES_DUN_EMPLOYE("groupes_dun_employe");

const QString YerithDatabaseTableColumn::GROUPES_DUN_EMPLOYE_ID("groupes_dun_employe_ID");

const QString YerithDatabaseTableColumn::HISTORIQUE_STOCK_ACTIF("historique_stock_actif");

const QString YerithDatabaseTableColumn::HISTORIQUE_STOCK("historique_stock");

const QString YerithDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID("membres_du_groupe_db_ID");

const QString YerithDatabaseTableColumn::POURCENTAGE_TAXES_IMPOSABLES("pourcentage_TAXES_IMPOSABLES");

const QString YerithDatabaseTableColumn::POURCENTAGE_DU_RABAIS("pourcentage_du_rabais");

const QString YerithDatabaseTableColumn::MAXIMUM_DE_MEMBRES("maximum_de_membres");

const QString YerithDatabaseTableColumn::CLIENTS_ID("clients_id");

const QString YerithDatabaseTableColumn::REFERENCE_RECU_PAIEMENT_CLIENT("reference_recu_paiement_client");

const QString YerithDatabaseTableColumn::REFERENCE_RECU_VENDU("reference_recu_vendu");

const QString YerithDatabaseTableColumn::REFERENCE_RECU_SORTIE("reference_recu_sortie");

const QString YerithDatabaseTableColumn::ENREGISTREUR_STOCK("enregistreur_stock");

const QString YerithDatabaseTableColumn::NOM_UTILISATEUR_DU_COMMANDEUR_DE_LACHAT("nom_utilisateur_du_commandeur_de_lachat");

const
QString
YerithDatabaseTableColumn::STATUT_DE_LACHAT_AU_FOURNISSEUR
("statut_de_lachat_au_fournisseur");

const QString YerithDatabaseTableColumn::memo_sale_for_sale("memo_sale_for_sale");

const QString YerithDatabaseTableColumn::REFERENCE("reference");

const QString YerithDatabaseTableColumn::COMPTE_FOURNISSEUR("compte_fournisseur");
const QString
YerithDatabaseTableColumn::COMPTE_CLIENT_PROGRAMME_DE_FIDELITE_CLIENTS
("compte_client_PROGRAMME_DE_FIDELITE_CLIENTS");
const
QString
YerithDatabaseTableColumn::COMPTECLIENT_AVANCE_DE_CASH
("compte_client_AVANCE_DE_CASH");
const
QString YerithDatabaseTableColumn::COMPTE_CLIENT("compte_client");
const QString
YerithDatabaseTableColumn::DETTE_MAXIMALE_COMPTE_CLIENT
("dette_maximale_compte_client");

const QString YerithDatabaseTableColumn::NUMERO_UNIQUE("numero_unique");

const QString YerithDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT("nom_departement_produit");
const
QString YerithDatabaseTableColumn::CATEGORIE("categorie");
const QString YerithDatabaseTableColumn::QUANTITE("quantite");
const QString YerithDatabaseTableColumn::STOCK_DALERTE("stock_dalerte");
const QString YerithDatabaseTableColumn::STOCK_MINIMUM("stock_minimum");
const QString YerithDatabaseTableColumn::STOCK_MAXIMUM("stock_maximum");
const QString YerithDatabaseTableColumn::QUANTITE_VENDUE("quantite_vendue");
const QString YerithDatabaseTableColumn::QUANTITE_TOTALE("quantite_totale");
const QString YerithDatabaseTableColumn::QUANTITE_SORTIE("quantite_sortie");
const QString YerithDatabaseTableColumn::DESCRIPTION_GROUPE("description_groupe");
const QString
YerithDatabaseTableColumn::DESCRIPTION_FOURNISSEUR
("description_fournisseur");
const
QString YerithDatabaseTableColumn::DESCRIPTION_PRODUIT("description_produit");
const QString YerithDatabaseTableColumn::PRIX_UNITAIRE("prix_unitaire");
const QString YerithDatabaseTableColumn::PRIX_UNITAIRE_EN_GROS("prix_unitaire_en_gros");
const QString YerithDatabaseTableColumn::REFERENCE_RECU_DACHAT("reference_recu_dachat");
const QString
YerithDatabaseTableColumn::PROGRAMME_DE_FIDELITE_CLIENTS
("programme_de_fidelite_clients");
const
QString YerithDatabaseTableColumn::PRIX_DACHAT_PRECEDENT("prix_dachat_precedent");
const QString YerithDatabaseTableColumn::PRIX_DACHAT("prix_dachat");
const QString YerithDatabaseTableColumn::PRIX_VENTE_PRECEDENT("prix_vente_precedent");

const QString YerithDatabaseTableColumn::PRIX_VENTE_EN_GROS_PRECEDENT("prix_vente_en_gros_precedent");

const QString YerithDatabaseTableColumn::PRIX_VENTE("prix_vente");

const QString YerithDatabaseTableColumn::PRIX_VENTE_EN_GROS("prix_vente_en_gros");

const QString YerithDatabaseTableColumn::REFERENCE_DU_COMPTE_BANCAIRE("reference_du_compte_bancaire");

const QString YerithDatabaseTableColumn::AGENCE_DU_COMPTE_BANCAIRE("agence_du_compte_bancaire");

const QString YerithDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE("intitule_du_compte_bancaire");

const QString YerithDatabaseTableColumn::
        INTITULE_COMPTE_DOPERATION_DANS_LIGNE_BUDGETAIRE("Intitule_compte_doperation_DANS_LIGNE_BUDGETAIRE");

const QString YerithDatabaseTableColumn::montant_RESTANT_de_la_ligne_budgetaire("montant_RESTANT_de_la_ligne_budgetaire");

const QString YerithDatabaseTableColumn::montant_de_la_ligne_budgetaire("montant_de_la_ligne_budgetaire");

const QString YerithDatabaseTableColumn::INTITULE_DE_LA_ligne_budgetaire("intitule_de_la_ligne_budgetaire");

const QString YerithDatabaseTableColumn::INSTITUT_BANCAIRE("institut_bancaire");

const QString YerithDatabaseTableColumn::DESCRIPTION_DU_COMPTE_BANCAIRE("description_du_compte_bancaire");

const QString YerithDatabaseTableColumn::DESCRIPTION_charge_financiere("description_charge_financiere");

const QString YerithDatabaseTableColumn::DESCRIPTION_ligne_budgetaire("description_ligne_budgetaire");

const QString YerithDatabaseTableColumn::DESCRIPTION_DEPARTEMENT_PRODUIT("description_departement_produit");

const QString YerithDatabaseTableColumn::DESCRIPTION_CATEGORIE("description_categorie");

const QString YerithDatabaseTableColumn::IS_SOUS_CATEGORIE("is_sous_categorie");

const QString YerithDatabaseTableColumn::MANAGER("manager");

const QString YerithDatabaseTableColumn::DEPARTEMENT("departement");

const QString YerithDatabaseTableColumn::POSITION_FONCTION("position_fonction");

const QString YerithDatabaseTableColumn::NOM_CATEGORIE("nom_categorie");

const QString YerithDatabaseTableColumn::LIGNE_BUDGETAIRE("ligne_budgetaire");

const QString YerithDatabaseTableColumn::MONTANT_TVA("montant_tva");

const QString YerithDatabaseTableColumn::MONTANT_TVA_EN_GROS("montant_tva_en_gros");

const QString YerithDatabaseTableColumn::TVA("tva");

const QString YerithDatabaseTableColumn::MARGE_BENEFICIAIRE("marge_beneficiaire");

const QString YerithDatabaseTableColumn::MARGE_BENEFICIAIRE_EN_GROS("marge_beneficiaire_en_gros");

const QString YerithDatabaseTableColumn::MONTANT_RECU("montant_recu");

const QString YerithDatabaseTableColumn::MONTANT_A_REMBOURSER("montant_a_rembourser");

const QString YerithDatabaseTableColumn::MONTANT_TOTAL_VENTE("montant_total_vente");

const QString YerithDatabaseTableColumn::MONTANT_RABAIS_PROGRAMME_DE_FIDELITE_CLIENTS("montant_rabais_programme_de_fidelite_clients");

const QString YerithDatabaseTableColumn::SERVICE_SUPPLEMENT_AJOUT_COMMERCIAL("service_supplement_ajout_commercial");

const QString YerithDatabaseTableColumn::REMISE_PRIX("remise_prix");

const QString YerithDatabaseTableColumn::REMISE_POURCENTAGE("remise_pourcentage");

const QString YerithDatabaseTableColumn::LOCALISATION("localisation");

const QString YerithDatabaseTableColumn::LOCALISATION_STOCK("localisation_stock");

const QString YerithDatabaseTableColumn::QUANTITE_PAR_LOT("quantite_par_lot");

const QString YerithDatabaseTableColumn::LOTS_ENTRANT("lots_entrant");

const QString YerithDatabaseTableColumn::EMPLOYE("employe");

const QString YerithDatabaseTableColumn::IMAGE_FOURNISSEUR("image_fournisseur");

const QString YerithDatabaseTableColumn::IMAGE_COMPTE_CLIENT("image_compte_client");

const QString YerithDatabaseTableColumn::LOGO_ENTREPRISE("logo_entreprise");

const QString YerithDatabaseTableColumn::IS_PAYEMENT_HR_HR("is_payement_hr_HR");

const QString YerithDatabaseTableColumn::IMAGE_PRODUIT("image_produit");

const QString YerithDatabaseTableColumn::DATE_DE_DEBUT_DAPPARTENANCE("date_de_debut_dappartenance");

const QString YerithDatabaseTableColumn::DATE_DE_FIN_DAPPARTENANCE("date_de_fin_dappartenance");

const QString YerithDatabaseTableColumn::DATE_PEREMPTION("date_peremption");

const QString YerithDatabaseTableColumn::DATE_DE_RECEPTION("date_de_reception");

const QString YerithDatabaseTableColumn::DATE_DE_COMMANDE("date_de_commande");

const QString YerithDatabaseTableColumn::DATE_ENTREE("date_entree");

const QString YerithDatabaseTableColumn::PRENOM("prenom");

const QString YerithDatabaseTableColumn::NOM("nom");

const QString YerithDatabaseTableColumn::NOM_RECEPTEUR("nom_recepteur");

const QString YerithDatabaseTableColumn::NOM_MAGASINIER("nom_magasinier");

const QString YerithDatabaseTableColumn::NOM_UTILISATEUR_MAGASINIER("nom_utilisateur_magasinier");

const QString YerithDatabaseTableColumn::NOM_UTILISATEUR_CAISSIER("nom_utilisateur_caissier");

const QString YerithDatabaseTableColumn::NOM_CAISSIER("nom_caissier");

const QString YerithDatabaseTableColumn::NOM_COMPLET_CREATEUR_REMISE_ALERTE("nom_complet_createur_remise_alerte");

const QString YerithDatabaseTableColumn::NOM_COMPLET("nom_complet");

const QString YerithDatabaseTableColumn::NOM_LOCALISATION("nom_localisation");

const QString YerithDatabaseTableColumn::MESSAGE_PDV_RECU_PETIT("message_PDV_recu_petit");

const QString YerithDatabaseTableColumn::NOM_UTILISATEUR("nom_utilisateur");

const QString YerithDatabaseTableColumn::STOCKS_ID("stocks_id");

const QString YerithDatabaseTableColumn::STOCK_MANAGER("stock_manager");

const QString YerithDatabaseTableColumn::MOT_PASSE("mot_passe");

const QString YerithDatabaseTableColumn::TITRE("titre");

const QString YerithDatabaseTableColumn::APPELATION_TITRE("appelation_titre");

const QString YerithDatabaseTableColumn::ROLE("role");

const QString YerithDatabaseTableColumn::NOM_ROLE("nom_role");

const QString YerithDatabaseTableColumn::EMAIL("email");

const QString YerithDatabaseTableColumn::REFERENCE_FOURNISSEUR("reference_fournisseur");

const QString YerithDatabaseTableColumn::REFERENCE_PROGRAMME_DE_FIDELITE_CLIENTS("reference_programme_de_fidelite_clients");

const QString YerithDatabaseTableColumn::REFERENCE_GROUPE("reference_groupe");

const QString YerithDatabaseTableColumn::REFERENCE_GROUPE_DE_PAIE_hr("reference_groupe_de_paie_hr");

const QString YerithDatabaseTableColumn::REFERENCE_GROUPE_DEMPLOYES_hr("reference_groupe_demployes_hr");

const QString YerithDatabaseTableColumn::REFERENCE_CLIENT("reference_client");

const QString YerithDatabaseTableColumn::NUMERO_TELEPHONE_1("numero_telephone_1");

const QString YerithDatabaseTableColumn::NUMERO_TELEPHONE_2("numero_telephone_2");

const QString YerithDatabaseTableColumn::LIEU_NAISSANCE("lieu_naissance");

const QString YerithDatabaseTableColumn::DATE_NAISSANCE("date_naissance");

const QString YerithDatabaseTableColumn::QUARTIER_ENTREPRISE_CLIENT("quartier_entreprise_client");

const QString YerithDatabaseTableColumn::VILLE_ENTREPRISE_CLIENT("ville_entreprise_client");

const QString YerithDatabaseTableColumn::VILLE("ville");

const QString YerithDatabaseTableColumn::QUARTIER("quartier");

const QString YerithDatabaseTableColumn::REGION_ETAT_ENTREPRISE_CLIENT("region_etat_entreprise_client");

const QString YerithDatabaseTableColumn::PROVINCE_ETAT("province_etat");

const QString YerithDatabaseTableColumn::PAYS("pays");

const QString YerithDatabaseTableColumn::ADRESSE("adresse");

const QString YerithDatabaseTableColumn::ADRESSE_IP("adresse_ip");

const QString YerithDatabaseTableColumn::ADRESSE_IP_RESEAU_IMPRIMANTE_THERMIQUE("adresse_ip_reseau_imprimante_thermique");

const QString YerithDatabaseTableColumn::BOITE_POSTALE("boite_postale");

const QString YerithDatabaseTableColumn::STATUT_RESOLU("statut_resolu");

const QString YerithDatabaseTableColumn::SIEGE_SOCIAL("siege_social");

const QString YerithDatabaseTableColumn::NOTES("notes");

const QString YerithDatabaseTableColumn::NOM_ENTREPRISE("nom_entreprise");

const QString YerithDatabaseTableColumn::SECTEURS_DACTIVITES("secteurs_dactivites");

const QString YerithDatabaseTableColumn::REFERENCE_REGISTRE_DU_COMMERCE("reference_registre_du_commerce");

const
QString
YerithDatabaseTableColumn::CARTE_DE_FIDELITE_CLIENT_nom_entreprise
("CARTE_DE_FIDELITE_CLIENT_nom_entreprise");

const
QString
YerithDatabaseTableColumn::CARTE_DE_FIDELITE_CLIENT_siege
("CARTE_DE_FIDELITE_CLIENT_siege");

const
QString
YerithDatabaseTableColumn::

CARTE_DE_FIDELITE_CLIENT_service_de_gestion_clientele
("CARTE_DE_FIDELITE_CLIENT_service_de_gestion_clientele");

const
QString
YerithDatabaseTableColumn::

CARTE_DE_FIDELITE_CLIENT_service_du_programme_de_fidelite_client
("CARTE_DE_FIDELITE_CLIENT_service_du_programme_de_fidelite_client");

const
QString
YerithDatabaseTableColumn::CARTE_DE_FIDELITE_CLIENT_email
("CARTE_DE_FIDELITE_CLIENT_email");

const
QString
YerithDatabaseTableColumn::CARTE_DE_FIDELITE_CLIENT_telephone
("CARTE_DE_FIDELITE_CLIENT_telephone");

const
QString YerithDatabaseTableColumn::NUMERO_CONTRIBUABLE("numero_contribuable");
const QString YerithDatabaseTableColumn::NOM_REPRESENTANT("nom_representant");
const QString YerithDatabaseTableColumn::NOM_ENTREPRISE_CLIENT("nom_entreprise_client");
const QString
YerithDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR
("nom_entreprise_fournisseur");

const QString YerithDatabaseTableColumn::DESCRIPTION_GROUPE_DE_PAIE_HR
("description_groupes_de_paie_hr");

const QString YerithDatabaseTableColumn::DESCRIPTION_GROUPE_DEMPLOYES_HR
("description_groupe_demployes_hr");

const QString YerithDatabaseTableColumn::DESCRIPTION_PROGRAMME_DE_FIDELITE_CLIENTS
("description_programme_de_fidelite_clients");

const
QString YerithDatabaseTableColumn::DESCRIPTION_CLIENT("description_client");
const QString YerithDatabaseTableColumn::DESIGNATION("designation");
const QString YerithDatabaseTableColumn::DESIGNATION_REMISE("designation_remise");
const QString YerithDatabaseTableColumn::DESIGNATION_ALERTE("designation_alerte");
const QString YerithDatabaseTableColumn::CONDITION_ALERTE("condition_alerte");
const QString YerithDatabaseTableColumn::NOM_ENCAISSEUR("nom_encaisseur");
const QString YerithDatabaseTableColumn::DATE_PAIEMENT("date_paiement");

const QString YerithDatabaseTableColumn::MONTANT_PAYE("montant_paye");

const QString YerithDatabaseTableColumn::MONTANT_A_PAYER_MENSUEL("MONTANT_A_PAYER_MENSUEL");

const QString YerithDatabaseTableColumn::DATE_VENTE("date_vente");
const QString YerithDatabaseTableColumn::DATE_DEBUT("date_debut");
const QString YerithDatabaseTableColumn::DATE_FIN("date_fin");
const QString YerithDatabaseTableColumn::DATE_SORTIE("date_sortie");
const QString YerithDatabaseTableColumn::HEURE_PAIEMENT("heure_paiement");
const QString YerithDatabaseTableColumn::HEURE_VENTE("heure_vente");
const QString YerithDatabaseTableColumn::HEURE_SORTIE("heure_sortie");
const QString YerithDatabaseTableColumn::DESTINATAIRE("destinataire");

const QString YerithDatabaseTableColumn::NOM_COMPLET_DESTINATAIRE("nom_complet_destinataire");

const QString YerithDatabaseTableColumn::REMISE_NOTES("remise_notes");

const QString YerithDatabaseTableColumn::MESSAGE_ALERTE("message_alerte");

const QString YerithDatabaseTableColumn::DATE_CREATION("date_creation");

const QString YerithDatabaseTableColumn::TYPE_DOPERATION_FINANCIERE("type_doperation_financiere");

const QString YerithDatabaseTableColumn::TYPE_DE_PAIEMENT("type_de_paiement");

const QString YerithDatabaseTableColumn::TYPE_DE_CREDIT_DEBIT("type_de_credit_debit");

const QString YerithDatabaseTableColumn::TYPE_DE_VENTE("type_de_vente");

const QString YerithDatabaseTableColumn::NOTIFICATIONS("notifications");

const QString YerithDatabaseTableColumn::REMISE_RESOLUE("remise_resolue");

const QString YerithDatabaseTableColumn::ALERTE_RESOLUE("alerte_resolue");

const QString YerithDatabaseTableColumn::LOCALISATION_SORTIE("localisation_sortie");

const QString YerithDatabaseTableColumn::LOCALISATION_ENTREE("localisation_entree");

const QString YerithDatabaseTableColumn::LOCALISATION_SORTIE_ID("localisation_sortie_id");

const QString YerithDatabaseTableColumn::LOCALISATION_ENTREE_ID("localisation_entree_id");


void YerithDatabaseTableColumn::RETRANSLATE()
{
    _tableColumnToUserViewString.clear();

    _tableColumnToUserViewString
        .insert(YerithDatabaseTableColumn::MONTANT_RABAIS_PROGRAMME_DE_FIDELITE_CLIENTS,
                QObject::tr("Rabais FIDÉLITÉ CLIENTS (%1)")
                  .arg(YerithERPConfig::currency));

    _tableColumnToUserViewString.insert(YerithDatabaseTableColumn::SERVICE_SUPPLEMENT_AJOUT_COMMERCIAL,
                                        QObject::tr("Service supplément (ajout commercial)"));

    _tableColumnToUserViewString.insert(YerithDatabaseTableColumn::REMISE_PRIX,
                                        QObject::tr("Remise (%1)").arg
                                        (YerithERPConfig::currency));

    _tableColumnToUserViewString.insert(YerithDatabaseTableColumn::IS_SERVICE,
                                        QObject::tr("Service"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::IS_VENTE_EN_GROS,
           QObject::tr("Vente en gros"));

    _tableColumnToUserViewString.insert(YerithDatabaseTableColumn::ID_ALERTE,
                                        QObject::tr("ID ALERTE"));

    _tableColumnToUserViewString.insert(YerithDatabaseTableColumn::ID,
                                        QObject::tr("ID"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::REFEREUR_CLIENT,
           QObject::tr("Référeur client"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::REFEREUR_CLIENT_ID,
           QObject::tr("ID Référeur client"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::GROUPES_DU_CLIENT,
           QObject::tr("Groupes du client"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::GROUPES_DU_CLIENT_ID,
           QObject::tr("Groupes du client (ID)"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::GROUPE_DE_PAIE_HR,
           QObject::tr("Groupe de paie (hr)"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::GROUPES_DUN_EMPLOYE,
           QObject::tr("Groupes d'1 employé"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::GROUPES_DUN_EMPLOYE_ID,
           QObject::tr("Groupes d'un employé (ID)"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::HISTORIQUE_STOCK_ACTIF,
           QObject::tr("Historique ACTIF"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::HISTORIQUE_STOCK,
           QObject::tr("Historique stock"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID,
           QObject::tr("Membres du groupe (leurs ID)"));

	_tableColumnToUserViewString.
	    insert(YerithDatabaseTableColumn::POURCENTAGE_TAXES_IMPOSABLES,
	       QObject::tr("TAXES IMPOSABLES (%)"));

	_tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::POURCENTAGE_DU_RABAIS,
           QObject::tr("Rabais (%)"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::MAXIMUM_DE_MEMBRES,
           QObject::tr("MAX membres"));

    _tableColumnToUserViewString.insert(YerithDatabaseTableColumn::CLIENTS_ID,
                                        QObject::tr("ID client"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::REFERENCE_RECU_PAIEMENT_CLIENT,
           QObject::tr("Réf. reçu paiement"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::REFERENCE_RECU_VENDU,
           QObject::tr("Réf. reçu vente"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::REFERENCE_RECU_SORTIE,
           QObject::tr("Réf. reçu transfert"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::ENREGISTREUR_STOCK,
           QObject::tr("Gestionnaire stock"));

    _tableColumnToUserViewString.insert
    (YerithDatabaseTableColumn::NOM_UTILISATEUR_DU_COMMANDEUR_DE_LACHAT,
     QObject::tr("Nom d'utilisateur du commandeur de l'achat"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::STATUT_DE_LACHAT_AU_FOURNISSEUR,
           QObject::tr("Statut achat fournisseur"));

    _tableColumnToUserViewString.insert(YerithDatabaseTableColumn::memo_sale_for_sale,
                                        QObject::tr("MÉMO POUR RECHERCHE"));

    _tableColumnToUserViewString.insert(YerithDatabaseTableColumn::REFERENCE,
                                        QObject::tr("Référence"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::COMPTE_FOURNISSEUR,
           QObject::tr("Compte fournisseur"));

    _tableColumnToUserViewString.insert
    (YerithDatabaseTableColumn::COMPTE_CLIENT_PROGRAMME_DE_FIDELITE_CLIENTS,
     QObject::tr("Compte FIDÉLITÉ CLIENT"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::COMPTECLIENT_AVANCE_DE_CASH,
           QObject::tr("Compte client (CASH avancé)"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::COMPTE_CLIENT,
           QObject::tr("Compte client"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::DETTE_MAXIMALE_COMPTE_CLIENT,
           QObject::tr("Dette maximale"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::NUMERO_UNIQUE,
           QObject::tr("Numéro unique"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
           QObject::tr("Département"));

    _tableColumnToUserViewString.insert(YerithDatabaseTableColumn::CATEGORIE,
                                        QObject::tr("Catégorie"));

    _tableColumnToUserViewString.insert(YerithDatabaseTableColumn::QUANTITE,
                                        QObject::tr("Quantité"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::STOCK_MINIMUM,
           QObject::tr("Stock minimum"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::STOCK_MAXIMUM,
           QObject::tr("Stock maximum"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::STOCK_DALERTE,
           QObject::tr("Stock d'alerte"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::QUANTITE_VENDUE,
           QObject::tr("Qté"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::QUANTITE_TOTALE,
           QObject::tr("Qté totale"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::QUANTITE_SORTIE,
           QObject::tr("Qté sortie"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::DESCRIPTION_GROUPE,
           QObject::tr("Description groupe"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::DESCRIPTION_FOURNISSEUR,
           QObject::tr("Description fournisseur"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::DESCRIPTION_PRODUIT,
           QObject::tr("Description produit"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::PRIX_UNITAIRE,
           QObject::tr("Prix unitaire"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::PRIX_UNITAIRE_EN_GROS,
           QObject::tr("Prix unitaire (en gros)"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::REFERENCE_RECU_DACHAT,
           QObject::tr("Référence achat"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::PROGRAMME_DE_FIDELITE_CLIENTS,
           QObject::tr("Programme de fidélité clients"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::PRIX_DACHAT_PRECEDENT,
           QObject::tr("Prix d'achat précédent"));

    _tableColumnToUserViewString.insert(YerithDatabaseTableColumn::PRIX_DACHAT,
                                        QObject::tr("Prix d'achat"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::PRIX_VENTE_PRECEDENT,
           QObject::tr("Prix vente précédent"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::PRIX_VENTE_EN_GROS_PRECEDENT,
           QObject::tr("P.V. (en gros) précédent"));

    _tableColumnToUserViewString.insert(YerithDatabaseTableColumn::PRIX_VENTE,
                                        QObject::tr("Prix vente (TTC)"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::PRIX_VENTE_EN_GROS,
           QObject::tr("Prix vente TTC (en gros)"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::REFERENCE_DU_COMPTE_BANCAIRE,
           QObject::tr("Réf. compte bancaire"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::AGENCE_DU_COMPTE_BANCAIRE,
           QObject::tr("Agence bancaire"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE,
           QObject::tr("Intitulé compte bancaire"));

    _tableColumnToUserViewString
		.insert(YerithDatabaseTableColumn::INTITULE_COMPTE_DOPERATION_DANS_LIGNE_BUDGETAIRE,
           QObject::tr("INTITULÉ LIGNE_BUDGETAIRE"));

    _tableColumnToUserViewString
		.insert(YerithDatabaseTableColumn::montant_RESTANT_de_la_ligne_budgetaire,
           QObject::tr("RESTE LIGNE BUDGÉTAIRE"));

    _tableColumnToUserViewString
		.insert(YerithDatabaseTableColumn::montant_de_la_ligne_budgetaire,
           QObject::tr("MONTANT LIGNE BUDGÉTAIRE"));

    _tableColumnToUserViewString
		.insert(YerithDatabaseTableColumn::INTITULE_DE_LA_ligne_budgetaire,
           QObject::tr("Intitulé ligne BUDGÉTAIRE"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::INSTITUT_BANCAIRE,
           QObject::tr("Institut compte bancaire"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::DESCRIPTION_DU_COMPTE_BANCAIRE,
           QObject::tr("Description compte bancaire"));

    _tableColumnToUserViewString
		.insert(YerithDatabaseTableColumn::DESCRIPTION_charge_financiere,
				QObject::tr("Description CHARGE FINANCIÈRE"));

    _tableColumnToUserViewString
		.insert(YerithDatabaseTableColumn::DESCRIPTION_ligne_budgetaire,
				QObject::tr("Description LIGNE BUDGÉTAIRE"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::DESCRIPTION_DEPARTEMENT_PRODUIT,
           QObject::tr("Description dép. produit"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::DESCRIPTION_CATEGORIE,
           QObject::tr("Description catégorie"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::IS_SOUS_CATEGORIE,
           QObject::tr("EST 1 catégorie"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::NOM_CATEGORIE,
           QObject::tr("Nom Catégorie"));

    _tableColumnToUserViewString
		.insert(YerithDatabaseTableColumn::LIGNE_BUDGETAIRE,
				QObject::tr("Ligne Budgétaire"));

    _tableColumnToUserViewString.insert(YerithDatabaseTableColumn::MONTANT_TVA,
                                        QObject::tr("Montant TVA"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::MONTANT_TVA_EN_GROS,
           QObject::tr("Montant TVA (en gros)"));

    _tableColumnToUserViewString.insert(YerithDatabaseTableColumn::TVA,
                                        QObject::tr("TVA"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::MARGE_BENEFICIAIRE,
           QObject::tr("Bénéfice"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::MARGE_BENEFICIAIRE_EN_GROS,
           QObject::tr("Bénéfice (gros)"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::MONTANT_RECU,
           QObject::tr("Montant reçu"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::MONTANT_A_REMBOURSER,
           QObject::tr("Montant à rembourser"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::MONTANT_TOTAL_VENTE,
           QObject::tr("Total vente"));

    _tableColumnToUserViewString.insert(YerithDatabaseTableColumn::SERVICE_SUPPLEMENT_AJOUT_COMMERCIAL,
                                        QObject::tr("Service supplément (Ajout commercial)"));

    _tableColumnToUserViewString.insert(YerithDatabaseTableColumn::REMISE_PRIX,
                                        QObject::tr("Remise prix"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::REMISE_POURCENTAGE,
           QObject::tr("Remise (%)"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::LOCALISATION,
           QObject::tr("Localisation"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::LOCALISATION_STOCK,
           QObject::tr("Localisation stock"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::QUANTITE_PAR_LOT,
           QObject::tr("Qté par lot"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::LOTS_ENTRANT,
           QObject::tr("Lots entrant"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::IMAGE_FOURNISSEUR,
           QObject::tr("Image fournisseur"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::IMAGE_COMPTE_CLIENT,
           QObject::tr("Image client"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::LOGO_ENTREPRISE,
           QObject::tr("Logo de l'entreprise"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::IS_PAYEMENT_HR_HR,
           QObject::tr("payement RH"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::IMAGE_PRODUIT,
           QObject::tr("Image produit"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::DATE_DE_DEBUT_DAPPARTENANCE,
           QObject::tr("Début d'appartenance"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::DATE_DE_FIN_DAPPARTENANCE,
           QObject::tr("Fin d'appartenance"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::DATE_PEREMPTION,
           QObject::tr("Date péremption"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::DATE_DE_RECEPTION,
           QObject::tr("Date réception"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::DATE_DE_COMMANDE,
           QObject::tr("Date commande"));

    _tableColumnToUserViewString.insert(YerithDatabaseTableColumn::DATE_ENTREE,
                                        QObject::tr("Date entrée"));

    _tableColumnToUserViewString.insert(YerithDatabaseTableColumn::PRENOM,
                                        QObject::tr("Prénom"));

    _tableColumnToUserViewString.insert(YerithDatabaseTableColumn::NOM,
                                        QObject::tr("Nom"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::NOM_RECEPTEUR,
           QObject::tr("Nom récepteur"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::NOM_MAGASINIER,
           QObject::tr("Nom magasinier"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::NOM_UTILISATEUR_MAGASINIER,
           QObject::tr("ID magasinier"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::NOM_UTILISATEUR_CAISSIER,
           QObject::tr("ID caissier"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::NOM_CAISSIER,
           QObject::tr("Nom caissier"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::NOM_COMPLET_CREATEUR_REMISE_ALERTE,
           QObject::tr("Créateur"));

    _tableColumnToUserViewString.insert(YerithDatabaseTableColumn::NOM_COMPLET,
                                        QObject::tr("Nom complet"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::NOM_LOCALISATION, QObject::tr("Nom"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::MESSAGE_PDV_RECU_PETIT,
           QObject::tr("MESSAGE PDV REÇU PETIT"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::NOM_UTILISATEUR, QObject::tr("ID"));

    _tableColumnToUserViewString.insert(YerithDatabaseTableColumn::STOCKS_ID,
                                        QObject::tr("Stocks ID"));

    _tableColumnToUserViewString.insert(YerithDatabaseTableColumn::MANAGER, QObject::tr("Manager"));

    _tableColumnToUserViewString.insert(YerithDatabaseTableColumn::DEPARTEMENT, QObject::tr("Département empl."));

    _tableColumnToUserViewString.insert(YerithDatabaseTableColumn::POSITION_FONCTION, QObject::tr("Position/fonction"));

    _tableColumnToUserViewString.insert(YerithDatabaseTableColumn::STOCK_MANAGER, QObject::tr("Stock manager"));

    _tableColumnToUserViewString.insert(YerithDatabaseTableColumn::MOT_PASSE,
                                        QObject::tr("Mot de passe"));

    _tableColumnToUserViewString.insert(YerithDatabaseTableColumn::TITRE,
                                        QObject::tr("Titre"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::APPELATION_TITRE,
           QObject::tr("Titre appelation"));

    _tableColumnToUserViewString.insert(YerithDatabaseTableColumn::ROLE,
                                        QObject::tr("rôle"));

    _tableColumnToUserViewString.insert(YerithDatabaseTableColumn::NOM_ROLE,
                                        QObject::tr("nom rôle"));

    _tableColumnToUserViewString.insert(YerithDatabaseTableColumn::EMAIL,
                                        QObject::tr("Émail"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::REFERENCE_FOURNISSEUR,
           QObject::tr("Référence fourn."));

    _tableColumnToUserViewString.insert
    (YerithDatabaseTableColumn::REFERENCE_PROGRAMME_DE_FIDELITE_CLIENTS,
     QObject::tr("Référence fidélité clients"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::REFERENCE_GROUPE,
           QObject::tr("Référence groupe"));

    _tableColumnToUserViewString.
        insert(YerithDatabaseTableColumn::REFERENCE_GROUPE_DE_PAIE_hr,
               QObject::tr("Référence gr. de paie (HR)"));

    _tableColumnToUserViewString.
        insert(YerithDatabaseTableColumn::REFERENCE_GROUPE_DEMPLOYES_hr,
               QObject::tr("Référence gr. d'employés (HR)"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::REFERENCE_CLIENT,
           QObject::tr("Référence client"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::NUMERO_TELEPHONE_1,
           QObject::tr("Numéro téléphone 1"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::NUMERO_TELEPHONE_2,
           QObject::tr("Numéro téléphone 2"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::LIEU_NAISSANCE,
           QObject::tr("Lieu naissance"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::DATE_NAISSANCE,
           QObject::tr("Date naissance"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::QUARTIER_ENTREPRISE_CLIENT,
           QObject::tr("Quartier client"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::VILLE_ENTREPRISE_CLIENT,
           QObject::tr("Ville client"));

    _tableColumnToUserViewString.insert(YerithDatabaseTableColumn::VILLE,
                                        QObject::tr("Ville"));

    _tableColumnToUserViewString.insert(YerithDatabaseTableColumn::QUARTIER,
                                        QObject::tr("Quartier"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::REGION_ETAT_ENTREPRISE_CLIENT,
           QObject::tr("Région/État client"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::PROVINCE_ETAT,
           QObject::tr("Province/État"));

    _tableColumnToUserViewString.insert(YerithDatabaseTableColumn::PAYS,
                                        QObject::tr("Pays"));

    _tableColumnToUserViewString.insert(YerithDatabaseTableColumn::ADRESSE,
                                        QObject::tr("Adresse"));

    _tableColumnToUserViewString.insert(YerithDatabaseTableColumn::ADRESSE_IP,
                                        QObject::tr("Adresse IP"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::ADRESSE_IP_RESEAU_IMPRIMANTE_THERMIQUE,
           QObject::tr("Adresse IP réseau imprimante ther."));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::BOITE_POSTALE,
           QObject::tr("Boîte postale"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::SIEGE_SOCIAL,
           QObject::tr("Siège social"));

    _tableColumnToUserViewString.insert(YerithDatabaseTableColumn::NOTES,
                                        QObject::tr("Notes"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::NOM_ENTREPRISE,
           QObject::tr("Nom Entreprise"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::SECTEURS_DACTIVITES,
           QObject::tr("Secteurs d'activités"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::REFERENCE_REGISTRE_DU_COMMERCE,
           QObject::tr("Régistre de commerce"));

    _tableColumnToUserViewString.insert
    (YerithDatabaseTableColumn::CARTE_DE_FIDELITE_CLIENT_nom_entreprise,
     QObject::tr("Nom de l'entreprise (carte)"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::CARTE_DE_FIDELITE_CLIENT_siege,
           QObject::tr("Siège (carte)"));

    _tableColumnToUserViewString.insert
    (YerithDatabaseTableColumn::
     CARTE_DE_FIDELITE_CLIENT_service_de_gestion_clientele,
     QObject::tr("GESTION CLIENTÈLE (carte)"));

    _tableColumnToUserViewString.insert
    (YerithDatabaseTableColumn::
     CARTE_DE_FIDELITE_CLIENT_service_du_programme_de_fidelite_client,
     QObject::tr("SERVICE DU PROGRAMME DE FIDÉLITÉ (carte)"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::CARTE_DE_FIDELITE_CLIENT_email,
           QObject::tr("Émail (carte)"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::CARTE_DE_FIDELITE_CLIENT_telephone,
           QObject::tr("Téléphone (carte)"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::NUMERO_CONTRIBUABLE,
           QObject::tr("Numéro contribuable"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::NOM_REPRESENTANT,
           QObject::tr("Nom représentant"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::NOM_ENTREPRISE_CLIENT,
           QObject::tr("Nom client"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR,
           QObject::tr("Nom fournisseur"));

    _tableColumnToUserViewString.insert
    (YerithDatabaseTableColumn::DESCRIPTION_PROGRAMME_DE_FIDELITE_CLIENTS,
     QObject::tr("Description prog. de fidélité clients"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::DESCRIPTION_CLIENT,
           QObject::tr("Description client"));

    _tableColumnToUserViewString.insert(YerithDatabaseTableColumn::DESIGNATION,
                                        QObject::tr("Désignation"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::DESIGNATION_REMISE,
           QObject::tr("Désignation remise"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::DESIGNATION_ALERTE,
           QObject::tr("Désignation alerte"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::CONDITION_ALERTE,
           QObject::tr("Condition alerte"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::NOM_ENCAISSEUR,
           QObject::tr("Nom encaisseur"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::DATE_PAIEMENT,
           QObject::tr("Date paiement"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::MONTANT_A_PAYER_MENSUEL,
           QObject::tr("À PAYER (mensuel)"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::MONTANT_PAYE,
           QObject::tr("Montant payé"));

    _tableColumnToUserViewString.insert(YerithDatabaseTableColumn::DATE_VENTE,
                                        QObject::tr("Date vente"));

    _tableColumnToUserViewString.insert(YerithDatabaseTableColumn::DATE_DEBUT,
                                        QObject::tr("Date début"));

    _tableColumnToUserViewString.insert(YerithDatabaseTableColumn::DATE_FIN,
                                        QObject::tr("Date fin"));

    _tableColumnToUserViewString.insert(YerithDatabaseTableColumn::DATE_SORTIE,
                                        QObject::tr("Date sortie"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::HEURE_PAIEMENT,
           QObject::tr("Heure paiement"));

    _tableColumnToUserViewString.insert(YerithDatabaseTableColumn::HEURE_VENTE,
                                        QObject::tr("Heure vente"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::HEURE_SORTIE,
           QObject::tr("Heure sortie"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::DESTINATAIRE,
           QObject::tr("Destinataire"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::NOM_COMPLET_DESTINATAIRE,
           QObject::tr("Nom complet destinataire"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::REMISE_NOTES, QObject::tr("Notes"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::MESSAGE_ALERTE,
           QObject::tr("Message d'alerte"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::DATE_CREATION,
           QObject::tr("Date de création"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::TYPE_DOPERATION_FINANCIERE,
           QObject::tr("Type d'opération comptable"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::TYPE_DE_PAIEMENT,
           QObject::tr("Type de paiement"));

    _tableColumnToUserViewString
        .insert(YerithDatabaseTableColumn::TYPE_DE_CREDIT_DEBIT,
                QObject::tr("entrée OU sortie"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::TYPE_DE_VENTE,
           QObject::tr("Type de vente"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::NOTIFICATIONS,
           QObject::tr("Notifications"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::REMISE_RESOLUE,
           QObject::tr("Remise résolue"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::ALERTE_RESOLUE,
           QObject::tr("Alerte résolue"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::LOCALISATION_SORTIE,
           QObject::tr("Local. sortie"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::LOCALISATION_ENTREE,
           QObject::tr("Local. entrée"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::LOCALISATION_SORTIE_ID,
           QObject::tr("ID local. sortie"));

    _tableColumnToUserViewString.
    insert(YerithDatabaseTableColumn::LOCALISATION_ENTREE_ID,
           QObject::tr("ID local. entrée"));

    //TODO: add an assertion to make sure that all keys, and values are unique
}
