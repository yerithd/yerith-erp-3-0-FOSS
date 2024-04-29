
void YerithAdminModifierWindow::setupEditRemise()
{
    _windowName = QString("%1 - %2")
    				.arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
                		 QObject::tr("administration ~ modifier ~ remises"));

    YerithAdminListerWindow *lw = _allWindows->_adminListerWindow;

    YerithSqlTableModel *remisesTableModel = lw->getCurSearchSqlTableModel();

    if (!remisesTableModel)
    {
        remisesTableModel = &_allWindows->getSqlTableModel_remises();
    }
    else if (remisesTableModel &&
             !YerithUtils::isEqualCaseInsensitive(remisesTableModel->
                                                  sqlTableName(),
                                                  YerithDatabase::REMISES))
    {
        remisesTableModel = &_allWindows->getSqlTableModel_remises();
    }

    QSqlRecord record =
                    remisesTableModel->record(lw->lastSelectedItemForModification());

}

void YerithAdminModifierWindow::modifier_remise()
{
    //_logger->log("modifier_remise");
    YerithAdminListerWindow *lw = _allWindows->_adminListerWindow;
    YerithSqlTableModel *remisesTableModel = lw->getCurSearchSqlTableModel();

    if (!remisesTableModel)
    {
        remisesTableModel = &_allWindows->getSqlTableModel_remises();
    }
    else if (remisesTableModel &&
             !YerithUtils::isEqualCaseInsensitive(remisesTableModel->
                                                  sqlTableName(),
                                                  YerithDatabase::REMISES))
    {
        remisesTableModel = &_allWindows->getSqlTableModel_remises();
    }

    QSqlRecord record =
                    remisesTableModel->record(lw->lastSelectedItemForModification());

}



bool YerithAdminModifierWindow::modifier_remise_check_fields(QString
                                                             previousDiscountText)
{
    return false;
}

void YerithAdminModifierWindow::clear_remise_all_fields()
{
}
