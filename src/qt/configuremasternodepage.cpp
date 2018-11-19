// Copyright (c) 2018 The Phore developers
// Copyright (c) 2018 The Curium developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#if defined(HAVE_CONFIG_H)
#include "config/curium-config.h"
#endif

#include "configuremasternodepage.h"
#include "ui_configuremasternodepage.h"

#include "activemasternode.h"
#include "bitcoingui.h"
#include "csvmodelwriter.h"
#include "editaddressdialog.h"
#include "guiutil.h"
#include "masternode-budget.h"
#include "masternode-payments.h"
#include "masternodeconfig.h"
#include "masternodeman.h"
#include "masternodelist.h"

#include <univalue.h>
#include <QIcon>
#include <QMenu>
#include <QString>
#include <QMessageBox>
#include <QSortFilterProxyModel>
#include <boost/tokenizer.hpp>
#include <fstream>
#include <iostream>
#include <string>

ConfigureMasternodePage::ConfigureMasternodePage(Mode mode, QWidget* parent) : QDialog(parent),
                                                                   ui(new Ui::ConfigureMasternodePage),
                                                                   mapper(0),
                                                                   mode(mode)
{
    ui->setupUi(this);
	
	GUIUtil::setupAliasWidget(ui->aliasEdit, this);
	GUIUtil::setupIPWidget(ui->vpsIpEdit, this);
	GUIUtil::setupPrivKeyWidget(ui->privKeyEdit, this);
	GUIUtil::setupTXIDWidget(ui->outputEdit, this);
	GUIUtil::setupTXIDIndexWidget(ui->outputIdEdit, this);

    switch (mode) {
    case NewConfigureMasternode:
        setWindowTitle(tr("New Masternode Alias"));
        break;
    case EditConfigureMasternode:
        setWindowTitle(tr("Edit Masternode Alias"));
        break;
    }

}

ConfigureMasternodePage::~ConfigureMasternodePage()
{
    delete ui;
}


void ConfigureMasternodePage::loadAlias(QString strAlias)
{
   ui->aliasEdit->setText(strAlias);
}

void ConfigureMasternodePage::counter(int counter)
{
   setCounters(counter);
}

void ConfigureMasternodePage::loadIP(QString strIP)
{
   ui->vpsIpEdit->setText(strIP);
}

void ConfigureMasternodePage::loadPrivKey(QString strPrivKey)
{
   ui->privKeyEdit->setText(strPrivKey);
}

void ConfigureMasternodePage::loadTxHash(QString strTxHash)
{
   ui->outputEdit->setText(strTxHash);
}

void ConfigureMasternodePage::loadOutputIndex(QString strOutputIndex)
{
   ui->outputIdEdit->setText(strOutputIndex);
}


void ConfigureMasternodePage::saveCurrentRow()
{

    switch (mode) {
    case NewConfigureMasternode:
		if(ui->aliasEdit->text().toStdString().empty() || ui->vpsIpEdit->text().toStdString().empty() || ui->privKeyEdit->text().toStdString().empty() || ui->outputEdit->text().toStdString().empty() || ui->outputIdEdit->text().toStdString().empty()) {
			break;
		}	
		masternodeConfig.add(ui->aliasEdit->text().toStdString(), ui->vpsIpEdit->text().toStdString(), ui->privKeyEdit->text().toStdString(), ui->outputEdit->text().toStdString(), ui->outputIdEdit->text().toStdString());
		masternodeConfig.writeToMasternodeConf();
        break;
    case EditConfigureMasternode:
		if(ui->aliasEdit->text().toStdString().empty() || ui->vpsIpEdit->text().toStdString().empty() || ui->privKeyEdit->text().toStdString().empty() || ui->outputEdit->text().toStdString().empty() || ui->outputIdEdit->text().toStdString().empty()) {
			break;
		}
		ConfigureMasternodePage::updateAlias(ui->aliasEdit->text().toStdString(), ui->vpsIpEdit->text().toStdString(), ui->privKeyEdit->text().toStdString(), ui->outputEdit->text().toStdString(), ui->outputIdEdit->text().toStdString());
		break;
    }
}

void ConfigureMasternodePage::accept()
{
	saveCurrentRow();
	emit accepted();
    QDialog::accept();
}


void ConfigureMasternodePage::updateAlias(std::string Alias, std::string IP, std::string PrivKey, std::string TxHash, std::string OutputIndex)
{

	int count = 0;
	count = getCounters();
	masternodeConfig.deleteAlias(count);
	masternodeConfig.add(Alias, IP, PrivKey, TxHash, OutputIndex);
	// write to masternode.conf
	masternodeConfig.writeToMasternodeConf();
			

}

void ConfigureMasternodePage::on_AutoFillPrivKey_clicked()
{
    CKey secret;
    secret.MakeNewKey(false);

	ui->privKeyEdit->setText(QString::fromStdString(CBitcoinSecret(secret).ToString()));
}


void ConfigureMasternodePage::on_AutoFillOutputs_clicked()
{
    // Find possible candidates
    vector<COutput> possibleCoins = activeMasternode.SelectCoinsMasternode();
	int test = 0;
    BOOST_FOREACH (COutput& out, possibleCoins) {
        std::string TXHash = out.tx->GetHash().ToString();
        std::string OutputID = std::to_string(out.i);
		BOOST_FOREACH (CMasternodeConfig::CMasternodeEntry mne, masternodeConfig.getEntries()) {
			
			if(TXHash == mne.getOutputIndex() && OutputID == mne.getTxHash()) {
				test = 1;
			}
		}
		if(test = 0) {
			ui->outputEdit->setText(QString::fromStdString(out.tx->GetHash().ToString()));
			ui->outputIdEdit->setText(QString::fromStdString(std::to_string(out.i)));		
			break;
		}				
    }	
}
