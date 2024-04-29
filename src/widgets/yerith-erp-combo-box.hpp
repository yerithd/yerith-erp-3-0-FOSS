/*
 * yerith-erp-combo-box.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef YERITH_COMBO_BOX_HPP_
#define YERITH_COMBO_BOX_HPP_


#include "src/utils/yerith-erp-utils.hpp"


#include <QtWidgets/QComboBox>

#include <QtWidgets/QLineEdit>

#include <QtSql/QSqlRecord>


class QSqlRecord;


class YerithComboBox : public QComboBox
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS

	YerithComboBox(QWidget *parent);

    inline ~YerithComboBox()
    {
    }

    void setupPopulateNORawString(QString 				aDBTableViewStringName,
                                  QString 				aDBFieldColumn,
                                  QMap<int, QString> 	*pointerToUserViewStringMAP);

    inline const QString &getDbFieldColumn() const
    {
        return _dbFieldColumn;
    }

    inline const QString &getDbTableViewStringName() const
    {
        return _dbTableViewStringName;
    }

    inline QMap<int, QString> *getPointerToUserViewStringMap()
    {
        return _pointerToUserViewStringMAP;
    }

    inline bool isPopulateRaw()
    {
        return _populateRawString;
    }

    bool checkField(bool red_only = false);

    void clearField();

    inline void resetYerithComboBox()
    {
        setCurrentIndex(0);
    }

    virtual inline bool contains(const QString &a_text)
    {
        return (-1 != findText(a_text));
    }

    inline QString currentText() const
    {
        return QComboBox::currentText().trimmed();
    }

    virtual inline bool isReadOnly()
    {
        return lineEdit()->isReadOnly();
    }

    virtual inline bool hasItem()
    {
        return (count() > 0);
    }

    inline bool isEmpty()
    {
        return currentText().isEmpty();
    }

    void setYerithEditable(bool editable);

    void setYerithEnabled(bool enabled);

    inline void saveRawCurrentValueToDatabase(const QString &aDBFieldColumn,
                                              QSqlRecord 	&aQSqlRecordToSAVE)
    {
        aQSqlRecordToSAVE.setValue(aDBFieldColumn, currentText());
    }

    void saveCurrentValueToDatabase(const QString 	&aDBFieldColumn,
                                    QSqlRecord 		&aQSqlRecordToSAVE);

    inline void setDBTableViewStringName(QString dbTableViewStringName)
    {
    	_dbTableViewStringName = dbTableViewStringName;
    }

    inline void setDBField(QString aDBFieldColumn)
    {
        _dbFieldColumn = aDBFieldColumn;
    }

    inline void setPointerToUserViewStringMAP(QMap<int, QString> *pointerToUserViewStringMAP)
    {
        _pointerToUserViewStringMAP = pointerToUserViewStringMAP;
    }

    virtual void find_AND_SET_CURRENT_INDEX_No_ADD_NEW_Item(const QString &a_text_TO_FIND_IN);

    virtual void find_AND_SET_CURRENT_INDEX(const QString &a_text_TO_FIND_IN);

    virtual void find_AND_SET_CURRENT_INDEX_NOT_EDITABLE(const QString &a_text_TO_FIND_IN);

    inline void yerith_remove_item(const QString &text)
    {
        removeItem(findText(text));
    }

    void addItems_AS_INITIALIZATION(const QString 		&current_configured_item,
                                    const QString 		&first_item_to_add,
                                    const QStringList 	&items_to_add_as_CHOICE);

    virtual void addItem_AFTER_POPULATE(const QString &text);

    virtual void addItem_AFTER_POPULATE(const QStringList &texts);

    virtual void addItems(const QStringList &texts);

    virtual void addItems(const QStringList 			&texts,
                          const QMap<QString, QString> 	&tableColumnToUserViewString);

    bool populateComboBoxRawString(QString aDBTableViewStringName,
                                   QString aDBFieldColumn,
                                   QString aConditionStr = "");

    bool populateComboBoxRawString_APPEND(QString aDBTableViewStringName,
                                          QString aDBFieldColumn,
                                          QString aConditionStr = "");

    bool populateComboBoxMissingRawString(const QString aDBFieldColumn,
                                          const QString aDBTableViewString,
                                          const QString aContentSTRINGValue);

    bool populateComboBox();

    bool populateComboBoxMissing(const int aContentINTValue);

    bool populateComboBoxWithout(const QList<int> &content_to_delete_from_user_view_int_values_IN_OUT);

    bool populateComboBoxWithout(const int aContentINTValue);


private:

    inline void setPopulateRaw(bool populateRawString)
    {
        _populateRawString = populateRawString;
    }

    bool _populateRawString;

    QString _dbTableViewStringName;

    QString _dbFieldColumn;

    QMap<int, QString> *_pointerToUserViewStringMAP;

    QPalette _originalPaletteBeforeMissingInformation;
};


#endif /* YERITH_COMBO_BOX_HPP_ */
