<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once 'Date.php';
require_once './includes/LN.LIB.functions.inc';
require_once 'class/roles.php';
require_once 'class/persons.php';
require_once 'class/user.php';
require_once 'class/client.php';
require_once 'class/loan_type.php';
require_once 'class/loan.php';

$loan = new LOAN($_REQUEST['id']);
$loan->load_borrower($loan->data['client_id']);
if (!count(WEBPAGE::$dbh->getAll(sprintf("select loan_id from tblLoansCurrentData where loan_id = %s", $loan->data['id'])))) {
  echo WEBPAGE::$gt['applyPayment.err']; exit; 	
  }

if ($loan->data['xp_pmt_date'] <= $loan->data['cn_date']) { 
  WEBPAGE::redirect(sprintf("index.popup.php?scr_name=LN.SCR.addPaymentDue&id=%s", $loan->data['id'])); 
  }

$tpl->setVariable('title', WEBPAGE::$gt['LN.SCR.addPayment']);

$tpl->setVariable('loan_code_label',		WEBPAGE::$gt['tblLoans.loan_code']);
$tpl->setVariable('loan_code',				$loan->data['loan_code']);
$tpl->setVariable('client_label',			WEBPAGE::$gt['tblClients.id']);
$tpl->setVariable('borrower_name',			$loan->data['borrower_name']);
$tpl->setVariable('borrower_code',			$loan->data['borrower_code']);

$tpl->setVariable('loan_id', 				$loan->data['id']);

$tpl->setVariable('pickOptionToContinue',	WEBPAGE::$gt['pickOptionToContinue']);
$tpl->setVariable('addPaymentEarly', 		WEBPAGE::$gt['LN.SCR.addPaymentEarly']);
$tpl->setVariable('addPaymentXT', 			WEBPAGE::$gt['LN.SCR.addPaymentXT']);
$tpl->setVariable('addPaymentFull', 		WEBPAGE::$gt['LN.SCR.addPaymentFull']);

$tpl->setVariable('message', 				WEBPAGE::printmessage('err',sprintf('%s<br>%s',WEBPAGE::$gt['noPendingPayments'],WEBPAGE::$gt['payEarly.inf'])));

?>