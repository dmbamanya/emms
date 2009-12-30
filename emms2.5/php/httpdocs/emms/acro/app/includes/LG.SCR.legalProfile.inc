<?php
require_once './includes/LN.LIB.functions.inc';
require_once 'Date.php';
require_once 'class/loan_type.php';
require_once 'class/loan.php';
require_once 'class/roles.php';
require_once 'class/client.php';
require_once 'class/society.php';
require_once 'class/business_type.php';
require_once 'class/business.php';

$loan  = new LOAN($_REQUEST['id']);
$loan->load_legal_data();

extract(LOAN::getTemplateData($_REQUEST['id']));

$btn  = new TTFButton(WEBPAGE::$theme);

$tpl->setVariable('title',					           WEBPAGE::$gt[WEBPAGE::$scr_name]);

$tpl->setVariable('general_info',					     WEBPAGE::$gt['general_info']);

$tpl->setVariable('loan_code',					       $loan_code);
$tpl->setVariable('loan_code_label',		       WEBPAGE::$gt['tblLoans.id']);
$tpl->setVariable('status',						         WEBPAGE::$gt[sprintf("tblLoans.status.%s",$status)]);
$tpl->setVariable('status_label',				       WEBPAGE::$gt['tblLoans.status']);
$tpl->setVariable('li_cause',						       $loan->data['legal']['cause']);
$tpl->setVariable('li_cause_label',				     WEBPAGE::$gt['tblLoansParked.category']);
$tpl->setVariable('borrower_name',				     $borrower_name);
$tpl->setVariable('borrower_name_label',	     WEBPAGE::$gt['tblClients.id']);
$tpl->setVariable('borrower_code',				     $borrower_code);
$tpl->setVariable('borrower_code_label',	     WEBPAGE::$gt['tblClients.code']);
$tpl->setVariable('currency',					         $currency);
$tpl->setVariable('currency_label',				     WEBPAGE::$gt['tblFunds.currency_id']);
$tpl->setVariable('modality',					         $modality);
$tpl->setVariable('modality_label',				     WEBPAGE::$gt['tblLoanTypes.description']);
$tpl->setVariable('kp',							           $kp);
$tpl->setVariable('kp_label',					         WEBPAGE::$gt['tblLoans.kp']);
$tpl->setVariable('currency_symbol',			     $currency_symbol);

// historial de saldos
$tpl->setVariable('transactions_resume',			 WEBPAGE::$gt['transactions_resume']);
$tpl->setVariable('transactions_resume_chart', WEBPAGE::printchart_ii($loan->data['legal']['transactions_resume'],array('total'=>WEBPAGE::$gt['total'],'principal'=>WEBPAGE::$gt['KP'],'insurance'=>WEBPAGE::$gt['INS'],'fees'=>WEBPAGE::$gt['FEES'],'interest'=>WEBPAGE::$gt['RT'],'penalties'=>WEBPAGE::$gt['PN'],'others'=>WEBPAGE::$gt['others'],'type'=>WEBPAGE::$gt['type'],'cycle'=>WEBPAGE::$gt['cycle']),array('totals'=>true,'cols'=>array('total','principal','insurance','fees','interest','penalties','others')),array(),array('legend'=>true)));

//  historial de pagos
$tpl->setVariable('payments',			             WEBPAGE::$gt['payment_history']);
$tpl->setVariable('payments_chart',            WEBPAGE::printchart_ii($loan->data['legal']['payments'],array('date'=>WEBPAGE::$gt['date'],'total'=>WEBPAGE::$gt['tblPayments.pmt'],'principal'=>WEBPAGE::$gt['KP'],'insurance'=>WEBPAGE::$gt['INS'],'fees'=>WEBPAGE::$gt['FEES'],'interest'=>WEBPAGE::$gt['RT'],'penalties'=>WEBPAGE::$gt['PN'],'others'=>WEBPAGE::$gt['others'],'username'=>WEBPAGE::$gt['tblUsers.username']),array('totals'=>true,'subtotals'=>true,'cols'=>array('total','principal','insurance','fees','interest','penalties','others')),array(),array('legend'=>true)));

// balance at the beginning of each cycle
$tpl->setVariable('ini_balances',			         WEBPAGE::$gt['ini_balances']);
$tpl->setVariable('ini_balances_chart',        WEBPAGE::printchart_ii($loan->data['legal']['ini_balances'],array('total'=>WEBPAGE::$gt['total'],'principal'=>WEBPAGE::$gt['KP'],'insurance'=>WEBPAGE::$gt['INS'],'fees'=>WEBPAGE::$gt['FEES'],'interest'=>WEBPAGE::$gt['RT'],'penalties'=>WEBPAGE::$gt['PN'],'date'=>WEBPAGE::$gt['date'],'cycle'=>WEBPAGE::$gt['cycle']),array(),array(),array('legend'=>true)));

// historial de cargos legales
$tpl->setVariable('write_off_charges',			   WEBPAGE::$gt['legal_charges']);
$tpl->setVariable('write_off_charges_chart', 	 WEBPAGE::printchart_ii($loan->data['legal']['dcharges'],array('date'=>WEBPAGE::$gt['date'],'amount'=>WEBPAGE::$gt['amount'],'description'=>WEBPAGE::$gt['description'],'username'=>WEBPAGE::$gt['tblUsers.username']),array('totals'=>true,'subtotals'=>false,'cols'=>array('amount')),array(),array('legend'=>true)));

// show applyPayment, write off and applyCharge buttons accordingly
if ($loan->data['status'] == 'LI') {
  if (WEBPAGE::validateFn('LG.SCR.addPayment')) { $tpl->setVariable('applyPayment', $btn->imglink(sprintf('%s?scr_name=LG.SCR.addPayment&id=%s',$_PHP_SELF,$loan->data['id']), WEBPAGE::$gt['applyPayment']));  }
  if ($loan->data['legal']['writeOff']) {
    if (WEBPAGE::validateFn('LG.SCR.addCharge')) { $tpl->setVariable('applyCharge', $btn->imglink(sprintf('%s?scr_name=LG.SCR.addCharge&id=%s',$_PHP_SELF,$loan->data['id']), WEBPAGE::$gt['applyLCharge']));  }
    } else {
    if (WEBPAGE::validateFn('LG.SCR.doWriteOff')) { $tpl->setVariable('doWriteOff', $btn->imglink(sprintf('%s?scr_name=LG.SCR.doWriteOff&id=%s',$_PHP_SELF,$loan->data['id']), WEBPAGE::$gt['writeOff']));  }
    }
  }

$tpl->setVariable('printerlink',			         WEBPAGE::printerlink());

?>