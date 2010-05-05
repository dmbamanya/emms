<?php
require_once './includes/LN.LIB.functions.inc';
require_once 'class/loan_master.php';
require_once 'class/loan_type.php';
require_once 'class/loan.php';
require_once 'class/client.php';
require_once 'class/society.php';
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';


$master = new LOAN_MASTER($_REQUEST['id']);
$loan   = new LOAN(0,$master->data['loan_type_id']);

$master->loadloans();
$master->loadBorrowerData();

$tpl->setVariable('borrower_label',      WEBPAGE::$gt['tblLoansMaster.borrower_id']);
$tpl->setVariable('borrower',            $master->data['borrower']->data['name']);
$tpl->setVariable('borrower_type_label', WEBPAGE::$gt['tblLoansMaster.borrower_type']);
$tpl->setVariable('borrower_type',       WEBPAGE::$gt[sprintf('tblLoanTypes.borrower_type.%s',$master->data['borrower_type'])]);
$tpl->setVariable('loan_type_label',     WEBPAGE::$gt['tblLoansMaster.loan_type_id']);
$tpl->setVariable('loan_type',           $loan->data['modality']);
$tpl->setVariable('amount_label',        WEBPAGE::$gt['tblLoansMaster.amount']);
$tpl->setVariable('amount',              $master->data['amount']);
$tpl->setVariable('check_number_label',  WEBPAGE::$gt['tblLoansMaster.check_number']);
$tpl->setVariable('check_number',        $master->data['check_number']);
$tpl->setVariable('check_status_label',  WEBPAGE::$gt['tblLoansMaster.check_status']);
$tpl->setVariable('check_status',        WEBPAGE::$gt[sprintf('tblLoansMaster.check_status.%s',$master->data['check_status'])]);
$tpl->setVariable('master_id',           $master->data['id']);
$tpl->setVariable('xp_dates',            WEBPAGE::$gt['xp_dates']);

if ($master->data['check_status'] == 'D')
{
    $tpl->setVariable('edit', sprintf(WEBPAGE::_HOTLINK_POPUP,WEBPAGE::$gt['LN.SCR.editCheckNumber'],'LN.SCR.editCheckNumber','id',$master->data['id'],'LN.SCR.editCheckNumber','no','yes','no','480','210',WEBPAGE::$gt['edit']));
}

if ($master->data['cancelled'])
{ 
  $tpl->setVariable('lnk_cancellation_letter', sprintf('<a href="index.popup.php?scr_name=LN.SCR.viewCancelLetter&id=%s" target="_blank"><img alt="%s" src="button.php?txt=%s"></a>',$_REQUEST['id'],WEBPAGE::$gt['cancellation_letter'],WEBPAGE::$gt['cancellation_letter']));
}

if (($master->data['check_status'] == 'D')&&(WEBPAGE::$conf['app']['contract_promissory_note'] == 1))
{
  $tpl->setVariable('lnk_promissory_note', sprintf('<a href="index.popup.php?scr_name=LN.SCR.promissoryNote&id=%s" target="_blank"><img alt="%s" src="button.php?txt=%s"></a>',$_REQUEST['id'],WEBPAGE::$gt['promissory_note'],WEBPAGE::$gt['promissory_note']));
  switch($master->data['borrower_type'])
  {
    case 'B':
	  $tpl->setVariable('lnk_contract', sprintf('<a href=javascript:openWin("index.popup.php?scr_name=LN.SCR.groupContract&id=%s","groupContract","menubar=no,scrollbars=yes,resizable=yes,width=800,height=320")>%s</a>',$_REQUEST['id'],WEBPAGE::$gt['loan_contract']));
	  break;
    default:
	  $tpl->setVariable('lnk_contract', sprintf('<a href=javascript:openWin("index.popup.php?scr_name=LN.SCR.individualContract&id=%s","individualContract","menubar=no,scrollbars=yes,resizable=yes,width=800,height=320")>%s</a>',$_REQUEST['id'],WEBPAGE::$gt['loan_contract']));
  }
}

if (WEBPAGE::$conf['app']['contract_select'] == 1 && $master->data['check_status'] != 'R')
{
  $contracts = WEBPAGE::$dbh->getAssoc(sprintf("select cn.tpl,cn.name from tblContracts cn where 1;"));

  $form = new HTML_QuickForm('showContract','GET','index.popup.php','_blank');

  $btn  = new TTFButton(WEBPAGE::$theme);

  $form->addElement('static', 	'submit', 	'', $btn->imgsubmit('showContract',WEBPAGE::$gt['printSheet']));
  $form->addElement('select',		'tpl',	'',	(array('-'=>'--- '.WEBPAGE::$gt['contract_select']) + $contracts),		'style="width:300px"');
  $form->addElement('hidden',		'id',			$_REQUEST['id']);
  $renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
  $form->accept($renderer);
}

if (WEBPAGE::$conf['app']['original_pmt_plan'] == 1)
{
  //$tpl->setVariable('lnk_original_payment_plan', sprintf('<a href=javascript:openWin("index.popup.php?scr_name=LN.SCR.originalPMT_plan&id=%s","LN.SCR.originalPMT_plan","menubar=no,scrollbars=yes,resizable=yes,width=800,height=320")>%s</a>',$_REQUEST['id'],WEBPAGE::$gt['LN.SCR.originalPMT_plan']));
  $tpl->setVariable('lnk_original_payment_plan', sprintf('<a href="index.popup.php?scr_name=LN.SCR.originalPMT_plan&id=%s" target="_blank"><img alt="%s" src="button.php?txt=%s"></a>',$_REQUEST['id'],WEBPAGE::$gt['LN.SCR.originalPMT_plan'],WEBPAGE::$gt['LN.SCR.originalPMT_plan']));
}

$tpl->setVariable('chart_title', WEBPAGE::$gt['details']);

/*
 * print sub-loans chart
 */
$head 	= array(WEBPAGE::$gt['tblLoans.loan_code'],WEBPAGE::$gt['tblLoans.kp'],WEBPAGE::$gt['tblClients.id'],WEBPAGE::$gt['status'],WEBPAGE::$gt['edit'], WEBPAGE::$gt['delete']);
$align 	= array('loan_code'=>'left','kp'=>'right','borrower'=>'left','status'=>'left','view'=>'center','edit'=>'center','delete'=>'center');
$mrow 	= WEBPAGE::$dbh->getAll(sprintf('select l.loan_code,l.kp,l.client_id,l.status,lmd.loan_id from tblLoans AS l, tblLoansMasterDetails AS lmd where lmd.master_id = %s AND l.id = lmd.loan_id',$_REQUEST['id']));

$tpl->addBlockfile('chart','chart','ST.chart.tpl');
foreach($head as $key=>$val)
{
  $tpl->setCurrentBlock("header") ;
  $tpl->setVariable('column_name', $val);
  $tpl->parseCurrentBlock("header") ;
}

foreach($mrow AS $key=>$val)
{
  $loan->load_borrower($val['client_id']);
  $data[$key]['loan_code'] 	= sprintf("<a title='%s' href='%s?scr_name=LN.SCR.viewILoan&id=%s'>%s</a>", WEBPAGE::$gt['tips']['view_loan_profile'],$PHP_SELF,$val['loan_id'],$val['loan_code']);
  $data[$key]['kp'] 		= sprintf(WEBPAGE::_HOTLINK_POPUP,WEBPAGE::$gt['tips']['view_pmt_plan'],'LN.SCR.pmtPlan','id',$val['loan_id'],'LN.SCR.pmtPlan','no','yes','no','780','400',$val['kp']);
  $data[$key]['borrower'] 	= sprintf(WEBPAGE::_HOTLINK_POPUP,WEBPAGE::$gt['tips']['view_credit_history'],'BS.SCR.viewClientXpress','id',$val['client_id'],'BS.SCR.viewClientXpress','no','yes','no','800','500',$loan->data['borrower_name']);
  $data[$key]['status'] 	= sprintf(WEBPAGE::_HOTLINK_POPUP,WEBPAGE::$gt['tips']['view_status_history'],'LN.RPT.loanStatusHistory','id',$val['loan_id'],'LN.RPT.loanStatusHistory','no','no','no','700','320',WEBPAGE::$gt[sprintf('tblLoans.status.%s',$val['status'])]);
  $data[$key]['edit'] 		= sprintf("<a href='%s?scr_name=LN.SCR.addILoan&id=%s'>%s</a>", $PHP_SELF,$val['loan_id'],WEBPAGE::$gt['edit']);
  $data[$key]['delete'] 	= sprintf("<a href='%s?scr_name=LN.SCR.deleteILoan&id=%s'>%s</a>", $PHP_SELF,$val['loan_id'],WEBPAGE::$gt['delete']);
}

foreach($data as $key=>$row)
{
  foreach($row as $col => $val)
  {
    $tpl->setCurrentBlock("row") ;
    $tpl->setVariable('align', $align[$col]);
    $tpl->setVariable('item', $val);
    $tpl->parseCurrentBlock("row") ;
  }
  $tpl->setCurrentBlock("results") ;
  $tpl->parseCurrentBlock("results") ;
}
$tpl->setCurrentBlock("chart") ;
$tpl->parseCurrentBlock("chart") ;

?>
