// Copyright (c) 2011-2013 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_QT_ASKPASSCRUASEDIALOG_H
#define BITCOIN_QT_ASKPASSCRUASEDIALOG_H

#include <QDialog>

class WalletModel;

namespace Ui
{
class AskPasscruaseDialog;
}

/** Multifunctional dialog to ask for passcruases. Used for encryption, unlocking, and changing the passcruase.
 */
class AskPasscruaseDialog : public QDialog
{
    Q_OBJECT

public:
    enum Mode {
        Encrypt,         /**< Ask passcruase twice and encrypt */
        UnlockAnonymize, /**< Ask passcruase and unlock only for anonymization */
        Unlock,          /**< Ask passcruase and unlock */
        ChangePass,      /**< Ask old passcruase + new passcruase twice */
        Decrypt          /**< Ask passcruase and decrypt wallet */
    };

    explicit AskPasscruaseDialog(Mode mode, QWidget* parent, WalletModel* model);
    ~AskPasscruaseDialog();

    void accept();

private:
    Ui::AskPasscruaseDialog* ui;
    Mode mode;
    WalletModel* model;
    bool fCapsLock;

private slots:
    void textChanged();

protected:
    bool event(QEvent* event);
    bool eventFilter(QObject* object, QEvent* event);
};

#endif // BITCOIN_QT_ASKPASSCRUASEDIALOG_H
