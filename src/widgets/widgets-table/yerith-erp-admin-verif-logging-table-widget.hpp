/*
 * yerith-erp-admin-verif-logging-table-widget.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_WIDGETS_YERITH_ADMIN_VERIF_LOGGING_TABLE_WIDGET_HPP_
#define SRC_WIDGETS_YERITH_ADMIN_VERIF_LOGGING_TABLE_WIDGET_HPP_

#include "src/widgets/widgets-table/yerith-erp-table-widget.hpp"

class YerithADMINVerifLoggingTableWidget: public YerithTableWidget
{
	Q_OBJECT

public:

	YERITH_CLASS_OPERATORS

	inline YerithADMINVerifLoggingTableWidget(QWidget *parent = 0)
	:YerithTableWidget(parent),
	 _TIMESTAMPtem(0),
	 _SIGNALItem(0),
	 _SOURCEItem(0),
	 _TARGETItem(0),
	 _changed_OR_modified_database_qty_Item(0)
	{
		setRowCount(12);
	}

	virtual inline ~YerithADMINVerifLoggingTableWidget()
	{
	}

	int addArticle(QString TIMESTAMPtem,
				   QString SIGNALItem,
				   QString SOURCEItem,
				   QString TARGETItem,
				   QString changed_OR_modified_database_qty_Item);

	static const unsigned int TIME_STAMP_COLUMN;

	static const unsigned int SIGNAL_COLUMN;

	static const unsigned int SOURCE_COLUMN;

	static const unsigned int TARGET_COLUMN;

	static const unsigned int changed_or_MODIFIED_database_QTY_COLUMN;

private:

	YerithQTableWidgetItem *_TIMESTAMPtem;

	YerithQTableWidgetItem *_SIGNALItem;

	YerithQTableWidgetItem *_SOURCEItem;

	YerithQTableWidgetItem *_TARGETItem;

	YerithQTableWidgetItem *_changed_OR_modified_database_qty_Item;
};

#endif //SRC_WIDGETS_YERITH_ADMIN_VERIF_LOGGING_TABLE_WIDGET_HPP_
