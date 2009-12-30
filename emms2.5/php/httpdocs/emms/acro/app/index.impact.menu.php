<?php
/*
 * Set cookie's secure flag if using ssl
 */
$_SERVER['HTTPS'] ? session_set_cookie_params(0,'/','',true,true) :  session_set_cookie_params(0,'/','',false,true);

error_reporting(E_ERROR);
require_once 'class/webpage.php';
require_once 'class/sql.php';
require_once 'PEAR.php';
require_once 'Auth.php';
require_once './includes/ST.LIB.login.inc';
require_once 'HTML/Template/ITX.php';
require_once 'class/TTFButton.php';
require_once "Spreadsheet/Excel/Writer.php";

WEBPAGE::START();

WEBPAGE::LOAD_SESSION();
WEBPAGE::$lang = isset($_GET['lang']) ? $_GET['lang'] : 'eng';
WEBPAGE::load_gt();

// check run mode
switch (WEBPAGE::$runMode) {
  case WEBPAGE::_RUN_MODE_OUTDATED:
    exit;
  }

switch ($_GET['option']) {
  case '1':
    $data = WEBPAGE::$dbh->getAll(sprintf("select id,category,question,answer_txt from tblSurveyItems where id > 9"));
    $head = array('id'=>'Question Code', 'category' => 'Category', 'question'=>'Questions','answer_txt'=>'Answers');
    $opt = "surveys.xls";
    break;
  case '2':
    $data = WEBPAGE::$dbh->getAll(sprintf("select sa.id,c.id client, c.gender, u.username advisor,sa.date,sa.answer_list from tblSurveyAnswers sa, tblUsers u, tblClients c where sa.survey_id = '2' and u.id = sa.advisor_id and c.id = sa.client_id"));
    $head = array('id'=>'ID', 'client' => 'Client', 'gender' => 'Cli Gender', 'advisor' => 'Loan Officer', 'date' => 'Date', 'answer_list' => 'Answers');
    $opt = "answers.xls";
    break;
  case '3':
    $data = WEBPAGE::$dbh->getAll(sprintf("select c.id,ELT(FIELD(c.education,'N','SP','P','SH','H','SC','C','SU','U','G'),'Illiterate','Elementary School','Middle School','Some High School','High School','Some Technical Institute','Technical Institute','Some University','University','Dominican-Haitian') education, ELT(FIELD(c.cstatus,'M','S','E','D','W'),'Married','Single','Free union','Divorced','Widow') cstatus,c.dependants,c.birthdate,c.gender,c.society_id,u.username,p.program,b.short_name branch from tblClients c, tblZones z, tblUsers u, tblPrograms p, tblZones b where u.id = c.advisor_id and z.id = c.zone_id and p.id = z.program_id and b.id = z.parent_id"));
    $head = array('client' => 'Client', 'education' => 'Education', 'cstatus' => 'Marital Status', 'dependants' => 'Dependants', 'birthdate' => 'Birthdate', 'gender' => 'Cli Gender', 'society_id' => 'Group', 'advisor' => 'Loan Officer', 'zone_id' => 'Branch');
    $opt = "current_clients.xls";
    break;
  case '4':
    $data = WEBPAGE::$dbh->getAll(sprintf("select c.id,ELT(FIELD(c.education,'N','SP','P','SH','H','SC','C','SU','U','G'),'Illiterate','Elementary School','Middle School','Some High School','High School','Some Technical Institute','Technical Institute','Some University','University','Dominican-Haitian') education, ELT(FIELD(c.cstatus,'M','S','E','D','W'),'Married','Single','Free union','Divorced','Widow') cstatus,c.dependants,c.birthdate,c.gender from tblClients c where c.zone_id = '0'"));
    $head = array('client' => 'Client','education' => 'Education', 'cstatus' => 'Marital Status', 'dependants' => 'Dependants', 'birthdate' => 'Birthdate', 'gender' => 'Cli Gender');
    $opt = "old_clients.xls";
    break;
  case '5':
    $data = WEBPAGE::$dbh->getAll(sprintf("select ciom.client_id,ciom.society_id,u.username,p.program,b.short_name branch,ciom.date,ELT(FIELD(ciom.cause,'A','B','C','D','E','F','G','H','I','J'),'Esperanza Graduate','Internal Graduate','Legal Process','Resigned','Dismissed','Temporary Dismissal','Temporary Licence','Volunteer','Moving','Health') cause from tblClientIOM ciom, tblZones z, tblUsers u, tblPrograms p, tblZones b where ciom.type = 'O' and ciom.internal = '0' and u.id = ciom.advisor_id and z.id = ciom.zone_id and p.id = z.program_id and b.id = z.parent_id"));
    $head = array('client' => 'Client', 'society_id' => 'Group', 'username' => 'LoanOfficer', 'program' => 'Program', 'branch' => 'Branch', 'date' => 'Date', 'cause' => 'Cause');
    $opt = "client_desertion.xls";
    break;
  case '6':
    $data = WEBPAGE::$dbh->getAll(sprintf("select b.id code,ELT(FIELD(bt.activity,'C','S','F','I'),'Commerce','Service','Farmer','Industry') activity,bt.type,b.name,c.id client,b.creator_date registered_on,b.editor_date last_edit_on from tblBusiness b, tblBusinessTypes bt, tblClients c where bt.id = b.type_id and c.id = b.client_list"));
    $head = array('code'=>'Code','activity' => 'Activity', 'type' => 'Type', 'name' => 'Name', 'client' => 'Client', 'registered_on' => 'Registered on', 'last_edit_on' => 'Last edit on');
    $opt = "business.xls";
    break;
  case '7':
    $data = WEBPAGE::$dbh->getAll(sprintf("select l.id code,ELT(FIELD(l.status,'N','R','O','S','A','D','G','C','LI','LO','RT','RO'),'New','Rejected','Open','Revised','Approved','Disbursed','Delivered','Cancelled','Legal process','Legal process solved','Retraction','Re-opened') status,l.client_id client,l.business_id business,l.installment,l.fees_at,l.fees_af,l.rates_r,l.rates_d,l.rates_e,l.margin_d,l.kp,l.kat,l.kaf,l.pmt,l.savings_p,l.savings_v,z.short_name branch,p.program program,u.username,l.delivered_date,l.first_payment_date,l.xp_cancel_date,l.xp_num_pmt from tblLoans l, tblUsers u, tblPrograms p, tblZones z where u.id = l.advisor_id and p.id = l.program_id and z.id = l.zone_id"));
    $head = array('code' => 'Code', 'status' => 'Status', 'client' => 'Client', 'business' => 'Business', 'installment' => 'Installment', 'fees_at' => 'Fees', 'fees_af' => 'Insurance','rates_r' => 'Interest','rates_d' => 'Penalties','rates_e' => 'Eff. Rate','margin_d' => 'PenaltiesGrace','kp' => 'Amount disbursed','kat' => 'Fees $','kaf' => 'Insurance $','pmt' => 'Cuotas','savings_p' => 'Compulsory savings','savings_v' => 'Voluntary savings','branch' => 'Branch','program' => 'Program','username' => 'Loan officer','delivered_date' => 'Disburse date','first_payment_date' => 'Expected 1st pmt date','xp_cancel_date' => 'Expected cancel date','xp_num_pmt' => 'Expected num. pmts' );
    $opt = "loans.xls";
    break;
  default:
?>
    <ul>
      <li><a href="index.impact.menu.php?option=1">Questions</a>
      <li><a href="index.impact.menu.php?option=2">Answers</a>
      <li><a href="index.impact.menu.php?option=3">Current Clients</a>
      <li><a href="index.impact.menu.php?option=4">Old Clients</a>
      <li><a href="index.impact.menu.php?option=5">Client desertion</a>
      <li><a href="index.impact.menu.php?option=6">Business</a>
      <li><a href="index.impact.menu.php?option=7">Loans</a>
    </ul>
<?
    exit;
  }

array_unshift($data,$head);

$xls =& new Spreadsheet_Excel_Writer();
$xls->send($opt);
$sheet =& $xls->addWorksheet('Sheet 1');

$row = 0;
foreach ( $data as $key => $val) {
  $col = 0;
   foreach ($val as $i => $j) {
   	  $sheet->write($row,$col,$j);
   	  $col++;
   	  }
 $row++;
}

$xls->close();
?>


