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
if ($loan->data['xp_pmt_date'] > $loan->data['cn_date']) {
  echo WEBPAGE::$gt['applyPayment.err']; exit;
  }

//# cuotas ya pagadas
$c = max(0,current(WEBPAGE::$dbh->getAssoc(sprintf("select loan_id,count(id) from tblPayments where loan_id = %s group by loan_id",$loan->data['id']))));
$oldest_pending = $c;
$totals = array('inx'=>WEBPAGE::$gt['total'],'date'=>'-','fees'=>0,'insurances'=>0,'principal'=>0,'interest'=>0,'delay'=>0,'penalties'=>0,'pmt'=>0,'balance_kp'=>'-');

$balance_kp							= $loan->data['balance_kp'];

while(($loan->data['xp_pmt_date'] <= $loan->data['cn_date'])&&($balance_kp>0)) {

  $data[$c]							= $loan->getNextPaymentData();

  $loan->data['xp_pmt_date']		= $data[$c]['xp_pmt_date'];
  $loan->data['xp_pmt']				= $data[$c]['xp_pmt'];
  $loan->data['balance_kp']			= $data[$c]['balance_kp'];
  $loan->data['balance_kaf']		= $data[$c]['balance_kaf'];
  $loan->data['balance_kat']		= $data[$c]['balance_kat'];
  $loan->data['r_from_date']		= $data[$c]['r_from_date'];

  $pmt[$c]['inx']					= $c + 1;
  $pmt[$c]['date']					= $data[$c]['pmt_date'];
  $pmt[$c]['fees']					= $data[$c]['fees'];
  $pmt[$c]['insurances']			= $data[$c]['insurances'];
  $pmt[$c]['principal']				= $data[$c]['principal'];
  $pmt[$c]['interest']				= $data[$c]['interest'];
  $pmt[$c]['delay']					= $data[$c]['delay'];
  $pmt[$c]['penalties']				= $data[$c]['penalties'];
  $pmt[$c]['pmt']					= $data[$c]['pmt'];
  $pmt[$c]['balance_kp']			= $data[$c]['balance_kp'];

  $totals['fees']					+= $data[$c]['fees'];
  $totals['insurances']				+= $data[$c]['insurances'];
  $totals['principal']				+= $data[$c]['principal'];
  $totals['interest']				+= $data[$c]['interest'];
  $totals['delay']					+= $data[$c]['delay'];
  $totals['penalties']				+= $data[$c]['penalties'];
  $totals['pmt']					+= $data[$c]['pmt'];

  $balance_kp 						=  $data[$c]['balance_kp'];
  $c++;

  }

$pmt[$c] 	= $totals;

$btn  = new TTFButton(WEBPAGE::$theme);
$form = new HTML_QuickForm('applyForm','POST');

$form->addElement('hidden',		'scr_name',			$_REQUEST['scr_name']);
$form->addElement('hidden',		'ref',				$_REQUEST['ref']);
$form->addElement('hidden',		'id',				$_REQUEST['id']);
$form->addElement('static', 	'submit', 			'', $btn->imgsubmit('applyForm',WEBPAGE::$gt['submit']));

$form->addElement('hidden',		'total_all',		$totals['pmt']);
$form->addElement('hidden',		'total_oldest',		$data[$oldest_pending]['pmt']);
$form->addElement('text',		'total_pmt',		WEBPAGE::$gt['total'],		'class=txtBold disabled');

$mode_radio[] = $form->createElement('radio',null,null,WEBPAGE::$gt['action.ApplyAllPendingPayments'],'All', 'class=radio onClick="document.applyForm.total_pmt.value=document.applyForm.total_all.value"');
$mode_radio[] = $form->createElement('radio',null,null,WEBPAGE::$gt['action.ApplyOldestPendingPayment'],'Oldest', 'class=radio onClick="document.applyForm.total_pmt.value=document.applyForm.total_oldest.value"');
$form->addGroup($mode_radio, 'mode', WEBPAGE::$gt['action'],' ');

$form->addElement('text',		'notes',		WEBPAGE::$gt['memo'], 'class=large');

WEBPAGE::$conf['app']['pmt_receipt_flag_a'] ?  $form->addElement('advcheckbox','pmt_receipt_flag_a',	'', WEBPAGE::$conf['app']['pmt_receipt_flag_a'],'class=checkbox') : '';
WEBPAGE::$conf['app']['pmt_receipt_flag_b'] ?  $form->addElement('advcheckbox','pmt_receipt_flag_b',	'', WEBPAGE::$conf['app']['pmt_receipt_flag_b'],'class=checkbox') : '';

if ($form->validate()) {

  $receipt_data = WEBPAGE::$dbh->getAll(sprintf("select max(p.id) as last_pID from tblPayments as p, tblLoansMasterDetails as lmd where lmd.master_id = %s and p.loan_id = lmd.loan_id", $loan->data['master_id']));

  switch ($_REQUEST['mode']) {
  	case 'Oldest':
  	  $sdata = array_shift($data);
  	  if (count($data)) {
  	  	$cn_data = array_shift($data);
  	  	$delay = $cn_data['delay'];
  	  	$penalties = $cn_data['penalties'];
  	    } else {
  	  	$delay = 0;
  	  	$penalties = 0;
  	    }
  	  $data = array();
  	  $data[$oldest_pending] = $sdata;
  	  unset($sdata);
  	  break;
  	case 'All':
  	  $delay = 0;
  	  $penalties = 0;
  	  break;
  	default:
  	  exit;
    }

  foreach($data as $key=>$val) {
  	WEBPAGE::$dbh->query(sprintf("insert into tblPayments (loan_id,date,pmt,penalties,delay,interest,insurances,fees,principal,balance_kp,balance_kaf,balance_kat,user_id) values ('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')",$loan->data['id'],$loan->data['cn_date'],$val['pmt'],$val['penalties'],$val['delay'],$val['interest'],$val['insurances'],$val['fees'],$val['principal'],$val['balance_kp'],$val['balance_kaf'],$val['balance_kat'],WEBPAGE::$userID));
  	$pmtIDs[] = mysql_insert_id(WEBPAGE::$dbh->connection);
    }
  $ldata = array_pop($data);
  WEBPAGE::$dbh->query(sprintf("update tblLoansCurrentData set balance_kp = '%s', balance_kaf = '%s', balance_kat = '%s', r_from_date = '%s', xp_pmt_date = '%s', xp_pmt = '%s', cn_delay = '%s', cn_penalties	= '%s' where loan_id = '%s'", $ldata['balance_kp'],$ldata['balance_kaf'],$ldata['balance_kat'],$ldata['r_from_date'],$ldata['xp_pmt_date'],$ldata['xp_pmt'],$delay,$penalties,$loan->data['id']));

  $ldata['balance_kp'] ? '' : $loan->cancelled();

// add receipt stuff
  if(( $n = count($pmtIDs)) > 0) {
  	$balance = WEBPAGE::$dbh->getAll(sprintf("select lmd.master_id, sum(lcd.balance_kp) balance_kp, sum(lcd.balance_kat) balance_kat, sum(balance_kaf) balance_kaf from tblLoansCurrentData lcd, tblLoansMasterDetails lmd where lmd.master_id = %s and lcd.loan_id = lmd.loan_id group by lmd.master_id",$loan->data['master_id']));
    if (count($balance) != 1) {
      $balance_kp = 0; $balance_kaf = 0; $balance_kat = 0;
      } else {
      $balance_kp = $balance[0]['balance_kp']; $balance_kaf = $balance[0]['balance_kaf']; $balance_kat = $balance[0]['balance_kat'];
      }
    $receipt_id = SQL::insert('tblReceipts',array('loanmaster_id'=>$loan->data['master_id'],'balance_kp'=>$balance_kp,'balance_kaf'=>$balance_kaf,'balance_kat'=>$balance_kat,'notes'=>$_REQUEST['notes'],'flag_a'=>$_REQUEST['pmt_receipt_flag_a'] ? 1 : 0,'flag_b'=>$_REQUEST['pmt_receipt_flag_b'] ? 1 : 0,'user_id'=>WEBPAGE::$userID,'date'=>date('Y-m-d')));
    $lnkData = array();
    foreach($pmtIDs as $key=>$val) {
      $lnkData[] = sprintf("(%s,%s)",$receipt_id,$val);
      }
    SQL::insert_mult('tblLinkReceiptsPayments','receipt_id,payment_id',implode(',',$lnkData));
    WEBPAGE::redirect(sprintf("index.popup.php?scr_name=LN.SCR.viewReceipt&id=%s",$receipt_id));
    }

//  WEBPAGE::redirect(sprintf("index.popup.php?scr_name=LN.SCR.paymentReceipt&master_id=%s&last_pID=%s",  $loan->data['master_id'],max(0,$receipt_data[0]['last_pID'])));

  } else {

  $tpl->setVariable('form_name',			WEBPAGE::$gt['LN.SCR.addPaymentDue']);
  $tpl->setVariable('loan_code_label',		WEBPAGE::$gt['tblLoans.loan_code']);
  $tpl->setVariable('loan_code',			$loan->data['loan_code']);
  $tpl->setVariable('client_label',			WEBPAGE::$gt['tblClients.id']);
  $tpl->setVariable('borrower_name',		$loan->data['borrower_name']);
  $tpl->setVariable('borrower_code',		$loan->data['borrower_code']);

  $head = array(WEBPAGE::$gt['tblPayments.id'],WEBPAGE::$gt['tblPayments.date'],WEBPAGE::$gt['tblPayments.fees'],WEBPAGE::$gt['tblPayments.insurances'],WEBPAGE::$gt['tblPayments.principal'],WEBPAGE::$gt['tblPayments.interest'],WEBPAGE::$gt['tblPayments.delay'],WEBPAGE::$gt['tblPayments.penalties'],WEBPAGE::$gt['tblPayments.pmt'],WEBPAGE::$gt['tblPayments.balance_kp']);
  $tpl->setVariable('due_chart', 		WEBPAGE::printchart($pmt,$head));

  $form->setDefaults(array('mode'=>'All', 'total_pmt'=>$totals['pmt']));
  $renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
  $renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
  $renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
  $form->accept($renderer);

  }

?>