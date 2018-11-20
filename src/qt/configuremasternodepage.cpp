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


void ConfigureMasternodePage::MNAliasCache(QString MnAliasCache)
{
	LogPrintf("Test 2.1 autofill outid2: %s\n", MnAliasCache.toStdString());
   setMnAliasCache(MnAliasCache.toStdString());
   LogPrintf("Test 2.2 autofill outid2: %s\n", MnAliasCache.toStdString());
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
		LogPrintf("Test 4.1 autofill outid2: \n");
		if(ui->aliasEdit->text().toStdString().empty() || ui->vpsIpEdit->text().toStdString().empty() || ui->privKeyEdit->text().toStdString().empty() || ui->outputEdit->text().toStdString().empty() || ui->outputIdEdit->text().toStdString().empty()) {
			break;
		}
		LogPrintf("Test 4.2 autofill outid2: \n");
		ConfigureMasternodePage::updateAlias(ui->aliasEdit->text().toStdString(), ui->vpsIpEdit->text().toStdString(), ui->privKeyEdit->text().toStdString(), ui->outputEdit->text().toStdString(), ui->outputIdEdit->text().toStdString());
		LogPrintf("Test 4.3 autofill outid2: \n");
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
	LogPrintf("Test 3.1 autofill outid2: \n");
	std::string MnAlias = getMnAliasCache();
	LogPrintf("Test 3.2 autofill outid2: %s\n", MnAlias);
	BOOST_FOREACH (CMasternodeConfig::CMasternodeEntry mne, masternodeConfig.getEntries()) {
		LogPrintf("Test 3.3 autofill outid2: \n");
		if(MnAlias == mne.getAlias()) {
			LogPrintf("Test 3.4 autofill outid2: \n");
			int count = 0;
			count = getCounters();
			vector<COutPoint> confLockedCoins;
			uint256 mnTxHash;
			mnTxHash.SetHex(TxHash);
			int nIndex;
			if(!mne.castOutputIndex(nIndex))
				continue;
			COutPoint outpoint = COutPoint(mnTxHash, nIndex);
			confLockedCoins.push_back(outpoint);
			pwalletMain->UnlockCoin(outpoint);

			masternodeConfig.deleteAlias(count);
			masternodeConfig.add(Alias, IP, PrivKey, TxHash, OutputIndex);
			// write to masternode.conf
			masternodeConfig.writeToMasternodeConf();
		}
	}	

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
    LogPrintf("Test 1.1 autofill: \n");
    BOOST_FOREACH (COutput& out, possibleCoins) {
        std::string TXHash = out.tx->GetHash().ToString();
        std::string OutputID = std::to_string(out.i);
        LogPrintf("Test 1.2 autofill txhash: %s\n", TXHash);
        LogPrintf("Test 1.3 autofill outid: %s\n", OutputID);
                BOOST_FOREACH (CMasternodeConfig::CMasternodeEntry mne, masternodeConfig.getEntries()) {
                        LogPrintf("Test 1.3 autofill outid2: %s\n", mne.getOutputIndex());
                        LogPrintf("Test 1.3 autofill txhash2: %s\n", mne.getTxHash());
                        if(OutputID == mne.getOutputIndex() && TXHash == mne.getTxHash()) {
                                test = 1;
                                LogPrintf("Test 1.4 helo helo autofill: %s\n");
                        }
                }

                LogPrintf("Test 1.5 autofill: %s\n", std::to_string(test));
                if(test == 0) {
                        ui->outputEdit->setText(QString::fromStdString(out.tx->GetHash().ToString()));
                        ui->outputIdEdit->setText(QString::fromStdString(std::to_string(out.i)));

                        break;
                }
                test = 0;
    }
}

