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
if (!count(WEBPAGE::$dbh->getAll(sprintf("select loan_id from tblLoansCurrentData where loan_id = %s", $loan->data['id']))))
{
  echo WEBPAGE::$gt['applyPayment.err']; exit;
}
if ($loan->data['xp_pmt_date'] < $loan->data['cn_date'])
{
  echo WEBPAGE::$gt['applyPayment.err']; exit;
}

/*
 * # cuotas ya pagadas
 */
$c = current(WEBPAGE::$dbh->getAssoc(sprintf("select loan_id,count(id) from tblPayments where loan_id = %s group by loan_id",$loan->data['id'])));

$totals = array('inx'=>WEBPAGE::$gt['total'],'date'=>'-','fees'=>0,'insurances'=>0,'principal'=>0,'interest'=>0,'delay'=>0,'penalties'=>0,'pmt'=>0,'balance_kp'=>'-');

$data = $loan->getNextPaymentData('pmt');

$loan->data['xp_pmt_date'] = $data['xp_pmt_date'];
$loan->data['xp_pmt']      = $data['xp_pmt'];
$loan->data['balance_kp']  = $data['balance_kp'];
$loan->data['balance_kaf'] = $data['balance_kaf'];
$loan->data['balance_kat'] = $data['balance_kat'];
$loan->data['r_from_date'] = $data['r_from_date'];

$pmt[$c]['inx']            = $c + 1;
$pmt[$c]['date']           = $data['pmt_date'];
$pmt[$c]['fees']           = $data['fees'];
$pmt[$c]['insurances']     = $data['insurances'];
$pmt[$c]['principal']      = $data['principal'];
$pmt[$c]['interest']       = $data['interest'];
$pmt[$c]['delay']          = $data['delay'];
$pmt[$c]['penalties']      = $data['penalties'];
$pmt[$c]['pmt']            = $data['pmt'];
$pmt[$c]['balance_kp']     = $data['balance_kp'];

$totals['fees']            = $data['fees'];
$totals['insurances']      = $data['insurances'];
$totals['principal']       = $data['principal'];
$totals['interest']        = $data['interest'];
$totals['delay']           = $data['delay'];
$totals['penalties']       = $data['penalties'];
$totals['pmt']             = $data['pmt'];

$pmt[$c+1] = $totals;

$btn  = new TTFButton(WEBPAGE::$theme);
$form = new HTML_QuickForm('applyForm','POST');

$form->addElement('hidden', 'scr_name',   $_REQUEST['scr_name']);
$form->addElement('hidden', 'ref',        $_REQUEST['ref']);
$form->addElement('hidden', 'id',         $_REQUEST['id']);
$form->addElement('text',   'notes',      WEBPAGE::$gt['memo'], 'class=large');
$form->addElement('static', 'submit', '', $btn->imgsubmit('applyForm',WEBPAGE::$gt['submit']));

if ($form->validate())
{
  $receipt_data = WEBPAGE::$dbh->getAll(sprintf("select max(p.id) as last_pID from tblPayments as p, tblLoansMasterDetails as lmd, tblLoans as l where lmd.master_id = %s and p.loan_id = lmd.loan_id", $loan->data['master_id']));

  WEBPAGE::$dbh->query(sprintf("insert into tblPayments (loan_id,date,pmt,penalties,delay,interest,insurances,fees,principal,balance_kp,balance_kaf,balance_kat,user_id) values ('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')",$loan->data['id'],$loan->data['cn_date'],$data['pmt'],$data['penalties'],$data['delay'],$data['interest'],$data['insurances'],$data['fees'],$data['principal'],$data['balance_kp'],$data['balance_kaf'],$data['balance_kat'],WEBPAGE::$userID));
  $pmtIDs[] = mysql_insert_id(WEBPAGE::$dbh->connection);
  WEBPAGE::$dbh->query(sprintf("update tblLoansCurrentData set balance_kp = '%s', balance_kaf = '%s', balance_kat = '%s', r_from_date = '%s', xp_pmt_date = '%s', xp_pmt = '%s', cn_delay = 0, cn_penalties	= 0 where loan_id = '%s'", $data['balance_kp'],$data['balance_kaf'],$data['balance_kat'],$data['r_from_date'],$data['xp_pmt_date'],$data['xp_pmt'],$loan->data['id']));

  $loan->data['balance_kp'] ? '' : $loan->cancelled();

/*
 *  add receipt stuff
 */
  if(( $n = count($pmtIDs)) > 0)
  {
    $balance = WEBPAGE::$dbh->getAll(sprintf("select lmd.master_id, sum(lcd.balance_kp) balance_kp, sum(lcd.balance_kat) balance_kat, sum(balance_kaf) balance_kaf from tblLoansCurrentData lcd, tblLoansMasterDetails lmd where lmd.master_id = %s and lcd.loan_id = lmd.loan_id group by lmd.master_id",$loan->data['master_id']));
    if (count($balance) != 1)
    {
      $balance_kp = 0; $balance_kaf = 0; $balance_kat = 0;
    } else {
      $balance_kp = $balance[0]['balance_kp']; $balance_kaf = $balance[0]['balance_kaf']; $balance_kat = $balance[0]['balance_kat'];
    }
    $receipt_id = SQL::insert('tblReceipts',array('loanmaster_id'=>$loan->data['master_id'],'balance_kp'=>$balance_kp,'balance_kaf'=>$balance_kaf,'balance_kat'=>$balance_kat,'notes'=>$_REQUEST['notes'],'flag_a'=>$_REQUEST['pmt_receipt_flag_a'] ? 1 : 0,'flag_b'=>$_REQUEST['pmt_receipt_flag_b'] ? 1 : 0,'user_id'=>WEBPAGE::$userID,'date'=>date('Y-m-d')));
    $lnkData = array();
    foreach($pmtIDs as $key=>$val)
    {
      $lnkData[] = sprintf("(%s,%s)",$receipt_id,$val);
    }
    SQL::insert_mult('tblLinkReceiptsPayments','receipt_id,payment_id',implode(',',$lnkData));
    WEBPAGE::redirect(sprintf("index.popup.php?scr_name=LN.SCR.viewReceipt&id=%s",$receipt_id));
  }

//  WEBPAGE::redirect(sprintf("index.popup.php?scr_name=LN.SCR.paymentReceipt&master_id=%s&last_pID=%s",  $loan->data['master_id'],max(0,$receipt_data[0]['last_pID'])));

} else {

  $tpl->setVariable('form_name',       WEBPAGE::$gt['LN.SCR.addPaymentEarly']);
  $tpl->setVariable('loan_code_label', WEBPAGE::$gt['tblLoans.loan_code']);
  $tpl->setVariable('loan_code',       $loan->data['loan_code']);
  $tpl->setVariable('client_label',    WEBPAGE::$gt['tblClients.id']);
  $tpl->setVariable('borrower_name',   $loan->data['borrower_name']);
  $tpl->setVariable('borrower_code',   $loan->data['borrower_code']);

  $head = array
          (
            'inx'        => WEBPAGE::$gt['tblPayments.id'],
            'date'       => WEBPAGE::$gt['tblPayments.date'],
            'fees'       => WEBPAGE::$gt['tblPayments.fees'],
            'insurances' => WEBPAGE::$gt['tblPayments.insurances'],
            'principal'  => WEBPAGE::$gt['tblPayments.principal'],
            'interest'   => WEBPAGE::$gt['tblPayments.interest'],
            'delay'      => WEBPAGE::$gt['tblPayments.delay'],
            'penalties'  => WEBPAGE::$gt['tblPayments.penalties'],
            'pmt'        => WEBPAGE::$gt['tblPayments.pmt'],
            'balance_kp' => WEBPAGE::$gt['tblPayments.balance_kp']
          );

  $tpl->setVariable('due_chart',       WEBPAGE::printchart($pmt,$head));

  $tpl->setVariable('total_pmt',       $totals['pmt']);
  $tpl->setVariable('total_pmt_label', WEBPAGE::$gt['total']);

  $form->setDefaults();
  $renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
  $renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
  $renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
  $form->accept($renderer);
}

?>

