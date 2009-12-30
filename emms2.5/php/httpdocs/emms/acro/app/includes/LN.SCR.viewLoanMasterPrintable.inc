<?php
require_once './includes/LN.LIB.functions.inc';
require_once 'class/loan_master.php';
require_once 'class/loan_type.php';
require_once 'class/loan.php';
require_once 'class/client.php';
require_once 'class/society.php';

$master 	= new LOAN_MASTER($_REQUEST['id']);   
$loan 		= new LOAN(0,$master->data['loan_type_id']);

$master->loadloans();
$master->loadBorrowerData();

$tpl->setVariable('page_title',                  WEBPAGE::$gt['LN.SCR.viewLoanMaster']);
$tpl->setVariable('zone_label',                  WEBPAGE::$gt['tblZones.zone']);
$tpl->setVariable('zone',                        $master->data['borrower']->data['zone']);
$tpl->setVariable('borrower_label',              WEBPAGE::$gt['tblLoansMaster.borrower_id']);
$tpl->setVariable('borrower',                    $master->data['borrower']->data['name']);
$tpl->setVariable('borrower_type_label',         WEBPAGE::$gt['tblLoansMaster.borrower_type']);
$tpl->setVariable('borrower_type',               WEBPAGE::$gt[sprintf('tblLoanTypes.borrower_type.%s',$master->data['borrower_type'])]);
$tpl->setVariable('loan_type_label',             WEBPAGE::$gt['tblLoansMaster.loan_type_id']);
$tpl->setVariable('loan_type',                   $loan->data['modality']);
$tpl->setVariable('amount_label',                WEBPAGE::$gt['tblLoansMaster.amount']);
$tpl->setVariable('amount',                      $master->data['amount']);
$tpl->setVariable('check_number_label',          WEBPAGE::$gt['tblLoansMaster.check_number']);
$tpl->setVariable('check_number',                $master->data['check_number']);
$tpl->setVariable('check_status_label',          WEBPAGE::$gt['tblLoansMaster.check_status']);
$tpl->setVariable('check_status',                WEBPAGE::$gt[sprintf('tblLoansMaster.check_status.%s',$master->data['check_status'])]);
$tpl->setVariable('xp_delivered_date',           WEBPAGE::verbose_date_format($master->data['xp_delivered_date']));
$tpl->setVariable('xp_delivered_date_label',     WEBPAGE::$gt['tblLoansMaster.xp_delivered_date']);
$tpl->setVariable('xp_first_payment_date',       WEBPAGE::verbose_date_format($master->data['xp_first_payment_date']));
$tpl->setVariable('xp_first_payment_date_label', WEBPAGE::$gt['tblLoansMaster.xp_first_payment_date']);
$tpl->setVariable('chart_title',                 WEBPAGE::$gt['details']);
$tpl->setVariable('date',                        date('Y-m-d'));
$tpl->setVariable('print',                       sprintf('<br><a href=javascript:makeRequest("%sindex.popup.php?scr_name=LN.SCR.loansMasterPrintCounter&id=%s")>%s</a>',WEBPAGE::$url,$_REQUEST['id'],WEBPAGE::$gt['printSheet']));

/*
 * print sub-loans chart
 */
$head 	= array('loan_code'=>WEBPAGE::$gt['tblLoans.loan_code'],'kp'=>WEBPAGE::$gt['tblLoans.kp'],'borrower'=>WEBPAGE::$gt['tblClients.id'],'status'=>WEBPAGE::$gt['status']);
$align 	= array('loan_code'=>'left','kp'=>'right','borrower'=>'left','status'=>'left');
$mrow 	= WEBPAGE::$dbh->getAll(sprintf('select l.loan_code,l.kp,l.client_id,l.status,lmd.loan_id from tblLoans l, tblLoansMasterDetails lmd where lmd.master_id = %s AND l.id = lmd.loan_id',$_REQUEST['id']));

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
  $data[$key]['loan_code'] 	= $val['loan_code'];
  $data[$key]['kp'] 		= sprintf(WEBPAGE::_HOTLINK_POPUP,WEBPAGE::$gt['tips']['view_pmt_plan'],'LN.SCR.pmtPlan','id',$val['loan_id'],'LN.SCR.pmtPlan','no','yes','no','780','400',$val['kp']);
  $data[$key]['borrower'] 	= sprintf(WEBPAGE::_HOTLINK_POPUP,WEBPAGE::$gt['tips']['view_credit_history'],'BS.SCR.viewClientXpress','id',$val['client_id'],'BS.SCR.viewClientXpress','no','yes','no','800','500',$loan->data['borrower_name']);
  $data[$key]['status'] 	= sprintf(WEBPAGE::_HOTLINK_POPUP,WEBPAGE::$gt['tips']['view_status_history'],'LN.RPT.loanStatusHistory','id',$val['loan_id'],'LN.RPT.loanStatusHistory','no','no','no','700','320',WEBPAGE::$gt[sprintf('tblLoans.status.%s',$val['status'])]);
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

if (count($master->get_duplicates()) > 0)
{
  $tpl->setVariable('master_id', $_REQUEST['id']);
  $tpl->setVariable('duplicates', WEBPAGE::$gt['verify_duplicates']);

  $tpl->addBlockfile('message','message','ST.SCR.message.tpl');
  $tpl->setCurrentBlock("message") ;
  $tpl->setVariable('ico',sprintf('%s/icons/wrn.png',WEBPAGE::$themePath));
  $tpl->setVariable('msg',WEBPAGE::$gt['duplicate_chk_request_alert']);
  $tpl->parseCurrentBlock("message") ;
}

?>