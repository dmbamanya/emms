<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once './includes/LN.LIB.functions.inc';
require_once 'class/roles.php';
require_once 'class/persons.php';
require_once 'class/client.php';
require_once 'class/user.php';
require_once 'class/loan_type.php';
require_once 'class/loan.php';
require_once 'class/stdo.php';

$form = new HTML_QuickForm('searchForm','POST');

$form->addElement('static', 'submit',    '', WEBPAGE::$btn->imgsubmit('searchForm',WEBPAGE::$gt['search']));
$form->addElement('static', 'reset',     '', WEBPAGE::$btn->imgreset('searchForm',WEBPAGE::$gt['reset']));
$form->addElement('static', 'cancel',    '', WEBPAGE::$btn->imglink($PHP_SELF,WEBPAGE::$gt['cancel']));
$form->addElement('hidden', 'scr_name',      $_REQUEST['scr_name']);

$form->addElement('select', 'zone_id',       WEBPAGE::$gt['tblUsers.zone_id'],           ($zones          = array('%%'=>'') + USER::zones()),               'class=large');
$form->addElement('select', 'advisor_id',    WEBPAGE::$gt['tblClients.advisor_id'],      ($advisors       = array('%%'=>'') + CLIENT::advisors()),          'class=large');
$form->addElement('select', 'borrower_type', WEBPAGE::$gt['tblLoanTypes.borrower_type'], ($borrower_types = array('%%'=>'') + LOAN_TYPE::borrower_types()), 'class=large');

if ($form->validate())
{ 
  $values = $form->_submitValues;

  if ($values['zone_id']        == '%%') { $operator_zone_id       = 'like'; } else { $operator_zone_id       = '='; }
  if ($values['borrower_type'] 	== '%%') { $operator_borrower_type = 'like'; } else { $operator_borrower_type = '='; }
  if ($values['advisor_id'] 	== '%%') { $operator_advisor_id    = 'like'; } else { $operator_advisor_id    = '='; }

  $extra['loan_code']['header']	= 'tblLoans.loan_code';
  $extra['loan_code']['pack']	= '%s';
  $extra['loan_code']['align']	= 'center';

  $extra['client']['header']    = 'tblClients.id';
  $extra['client']['pack']      = '%s';
  $extra['client']['align']     = 'left';

  $extra['hits']['header']      = 'payments';
  $extra['hits']['pack']        = '%s';
  $extra['hits']['align']       = 'center';

  $extra['delay']['header']     = 'delay';
  $extra['delay']['pack']       = '%s';
  $extra['delay']['align']      = 'center';

  $extra['view']['header']      = 'view';
  $extra['view']['pack']        = sprintf("<a href='%s?scr_name=LN.SCR.viewILoan&ref=LN.SCR.browseLoans&id=%s'>%s</a>", $PHP_SELF,'%s',WEBPAGE::$gt['view']);
  $extra['view']['align']       = 'center';

  $extra['apply']['header']     = 'applyPayment';
  $extra['apply']['pack']       = sprintf("<a href=\"javascript:openWin('index.popup.php?scr_name=LN.SCR.addPaymentDue&id=%s','addPayment','menubar=no,scrollbars=yes,resizable=yes,width=700,height=320')\">%s</a>","%s",WEBPAGE::$gt['applyPayment']);
  $extra['apply']['align']      = 'center';

  
  $extra['param']['value']['zone_id']                    = $values['zone_id'];
  $extra['param']['value']['advisor_id']                 = $values['advisor_id'];
  $extra['param']['value']['borrower_type']              = $values['borrower_type'];

  $extra['param']['label']['tblUsers.zone_id']           = $zones[$values['zone_id']]                ? $zones[$values['zone_id']]                : WEBPAGE::$gt['all'];
  $extra['param']['label']['tblClients.advisor_id']      = $advisors[$values['advisor_id']]          ? $advisors[$values['advisor_id']]          : WEBPAGE::$gt['all'];
  $extra['param']['label']['tblLoanTypes.borrower_type'] = $borrower_types[$values['borrower_type']] ? $borrower_types[$values['borrower_type']] : WEBPAGE::$gt['all'];

  $extra['param']['ref'] = WEBPAGE::$scr_name;
  $extra['param']['ts']  = microtime(true);
  
  $mrow = WEBPAGE::$dbh->getAll(sprintf("
            select
              l.loan_code,
              concat(c.first, ' ', c.last) client,
              lod.hits, lod.delay,
              lod.loan_id view,
              lod.loan_id apply
            from
              tblLoansOnDelinquency lod,
              tblLoans l,
              tblLoanTypes lt,
              tblLoansCurrentData lcd,
              tblClients c
            where
              c.id              = l.client_id    and
              lod.date          = %s             and
              l.id              = lod.loan_id    and
              l.zone_id        %s '%s'           and
              l.advisor_id     %s '%s'           and
              lt.borrower_type %s '%s'           and
              lt.id             = l.loan_type_id and
              lcd.loan_id       = lod.loan_id    and
              lcd.xp_pmt_date  <= %s", 'curdate()',
                                       $operator_zone_id,       $values['zone_id'],
                                       $operator_advisor_id,    $values['advisor_id'],
                                       $operator_borrower_type, $values['borrower_type'],
                                       'curdate()'));

  $prow = WEBPAGE::$dbh->getAssoc(sprintf("
            select
              p.loan_id,
              count(p.id) hits,
              sum(p.delay) delay
            from
              tblPayments p,
              tblLoansCurrentData lcd
            where
              p.date           = %s          and
              p.loan_id        = lcd.loan_id and
              lcd.xp_pmt_date <= %s
           group by
              p.loan_id",'curdate()','curdate()'));

  foreach($mrow as $key=>$val)
  {
    $mrow[$key]['hits']  = $val['hits']  - ($prow[$val['view']]['hits']  ? $prow[$val['view']]['hits']  : 0);
    $mrow[$key]['delay'] = $val['delay'] - ($prow[$val['view']]['delay'] ? $prow[$val['view']]['delay'] : 0);
  }

  array_unshift($mrow,$extra);

  WEBPAGE::makecachefile($mrow,WEBPAGE::$queryCache.'.'.$extra['param']['ts']);
  require_once 'BS.SCR.pager.inc';
  
} else {

  $form->setDefaults();
  $renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
  $renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
  $renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
  $tpl->setVariable('content_title',WEBPAGE::$gt[WEBPAGE::$scr_name]);
  $form->accept($renderer);
}

?>