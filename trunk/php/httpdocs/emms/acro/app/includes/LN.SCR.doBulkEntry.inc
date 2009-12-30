<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once 'class/persons.php';
require_once 'class/roles.php';
require_once 'class/user.php';
require_once 'class/society.php';
require_once 'class/loan_type.php';

$ltype   = new LOAN_TYPE();
$society = new SOCIETY();

$form = new HTML_QuickForm('searchForm','POST');

$form->addElement('static', 'submit', '', WEBPAGE::$btn->imgsubmit('searchForm',WEBPAGE::$gt['search']));
$form->addElement('static', 'reset',  '', WEBPAGE::$btn->imgreset('searchForm',WEBPAGE::$gt['reset']));
$form->addElement('static', 'cancel', '', WEBPAGE::$btn->imglink($PHP_SELF,WEBPAGE::$gt['cancel']));
$form->addElement('hidden', 'scr_name',   $_REQUEST['scr_name']);

$form->addElement('text', 'code', WEBPAGE::$gt['tblSocieties.code'], 'class=large');
$form->addElement('text', 'name', WEBPAGE::$gt['tblSocieties.name'], 'class=large');

$form->addElement('select', 'zone_id',    WEBPAGE::$gt['tblSocieties.zone_id'],    ($zones = (array(''=>'') + $society->zones())),      'class=large');
$form->addElement('select', 'advisor_id', WEBPAGE::$gt['tblSocieties.advisor_id'], ($advisors = (array(''=>'') + USER::advisors('G'))), 'class=large');

$form->addRule('zone_id',    WEBPAGE::$gt['selectOption'],'required');
$form->addRule('zone_id',    WEBPAGE::$gt['selectOption'],'numeric');
$form->addRule('advisor_id', WEBPAGE::$gt['selectOption'],'required');
$form->addRule('advisor_id', WEBPAGE::$gt['selectOption'],'numeric');

if ($form->validate())
{
  $values = $form->_submitValues;

  $extra['param']['value']['name']       = $values['name'];
  $extra['param']['value']['code']       = $values['code'];
  $extra['param']['value']['zone_id']    = $values['zone_id'];
  $extra['param']['value']['advisor_id'] = $values['advisor_id'];

  $extra['param']['label']['tblSocieties.name']       = $values['name']       ? $values['name']                  : WEBPAGE::$gt['all'];
  $extra['param']['label']['tblSocieties.code']       = $values['code']       ? $values['code']                  : WEBPAGE::$gt['all'];
  $extra['param']['label']['tblSocieties.zone_id']    = $values['zone_id']    ? $zones[$values['zone_id']]       : WEBPAGE::$gt['all'];
  $extra['param']['label']['tblSocieties.advisor_id'] = $values['advisor_id'] ? $advisors[$values['advisor_id']] : WEBPAGE::$gt['all'];

  $extra['param']['ref'] = 'LN.SCR.doBulkEntry';
  $extra['param']['ts']  = microtime(true);

  $fields[]                = "name";
  $extra['name']['header'] = 'tblSocieties.name';
  $extra['name']['pack']   = '%s';
  $extra['name']['align']  = 'left';

  $fields[]                = "code";
  $extra['code']['header'] = 'tblSocieties.code';
  $extra['code']['pack']   = '%s';
  $extra['code']['align']  = 'center';

  $fields[]                  = "lm.id as lmcode";
  $extra['lmcode']['header'] = 'details';
  $extra['lmcode']['pack']   = '%s';
  $extra['lmcode']['align']  = 'right';

  $fields[]                    = "cn_delay";
  $extra['cn_delay']['header'] = 'tblPayments.delay';
  $extra['cn_delay']['pack']   = '%s';
  $extra['cn_delay']['align']  = 'center';

  $fields[]                = 'id as view';
  $extra['view']['header'] = 'view';
  $extra['view']['pack']   = sprintf("<a href='%s?scr_name=BS.SCR.viewSociety&ref=BS.SCR.browseSocieties&id=%s'>%s</a>", $PHP_SELF,'%s',WEBPAGE::$gt['view']);
  $extra['view']['align']  = 'center';

  $fields[]                       = 'lm.id as doBulkEntry';
  $extra['doBulkEntry']['header'] = 'select';
  $extra['doBulkEntry']['pack']   = sprintf('<a alt="" onClick="" href=javascript:openWin("index.popup.php?scr_name=LN.SCR.addPaymentBulk&id=%s","viewBulkEntry","menubar=no,scrollbars=yes,resizable=yes,width=800,height=320")>%s</a>','%s',WEBPAGE::$gt['select']);
  $extra['doBulkEntry']['align']  = 'center';

  $mrow = WEBPAGE::$dbh->getAll(sprintf("
            select
              s.name,
              concat(lpad(s.zone_id,3,'0'),'.',s.category,'.',lpad(s.id,5,'0')) code,
              lm.id lmcode,
              lcd.cn_delay,
              s.id view,
              lm.id doBulkEntry
            from
              tblSocieties s,
              tblLoansCurrentData lcd,
              tblLoans l,
              tblLoansMasterDetails lmd,
              tblLoansMaster lm
            where
              lm.check_status  = 'R'           and
              lmd.loan_id      = lcd.loan_id   and
              l.id             = lcd.loan_id   and
              lcd.xp_pmt_date <= lcd.cn_date   and
              lcd.cn_delay    <= l.margin_d    and
              lm.id            = lmd.master_id and
              lm.borrower_type = 'B'           and
              lm.borrower_id   = s.id          and
              s.zone_id        = %s            and
              s.advisor_id     = %s            and
              s.name           like '%%%s%%'
            group by
              lm.id
            having
              code like '%%%s%%'
            order by
              s.name,
              lcd.cn_delay desc", $values['zone_id'],
                                  $values['advisor_id'],
                                  $values['name'],
                                  $values['code']));

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
