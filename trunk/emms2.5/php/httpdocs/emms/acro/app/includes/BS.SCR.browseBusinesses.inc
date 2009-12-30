<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once 'class/roles.php';
require_once 'class/persons.php';
require_once 'class/client.php';
require_once 'class/business_type.php';
require_once 'class/business.php';

$bz = new BUSINESS();

$form = new HTML_QuickForm('searchForm','POST');

$form->addElement('static', 'submit', '', WEBPAGE::$btn->imgsubmit('searchForm',WEBPAGE::$gt['search']));
$form->addElement('static', 'reset',  '', WEBPAGE::$btn->imgreset('searchForm',WEBPAGE::$gt['reset']));
$form->addElement('static', 'cancel', '', WEBPAGE::$btn->imglink($PHP_SELF,WEBPAGE::$gt['cancel']));
$form->addElement('hidden', 'scr_name',   $_REQUEST['scr_name']);

$form->addElement('text',   'code',       WEBPAGE::$gt['tblClients.code'],           'class=large');
$form->addElement('text',   'first',      WEBPAGE::$gt['tblClients.first'],          'class=large');
$form->addElement('text',   'last',       WEBPAGE::$gt['tblClients.last'],           'class=large');
$form->addElement('select', 'activity',   WEBPAGE::$gt['tblBusinessTypes.activity'], ($activity = array('') + $bz->activities()), 'class=large onChange="if (this.value == 0) { searchForm.type_id.disabled=false; } else { searchForm.type_id.value=0;searchForm.type_id.disabled=true; }"');
$form->addElement('select', 'type_id',    WEBPAGE::$gt['tblBusiness.type_id'],       ($type_id = array('') + $bz->types()),       'class=large');

$form->addRule('code',                    WEBPAGE::$conf['app']['regex_govID_tip'],  'regex', WEBPAGE::$conf['app']['regex_govID']);

if ($form->validate())
{
  $values = $form->_submitValues;

  $extra['param']['value']['code']     = $values['code'];
  $extra['param']['value']['activity'] = $values['activity'];
  $extra['param']['value']['type_id']  = $values['type_id'];
  $extra['param']['value']['first']    = $values['first'];
  $extra['param']['value']['last']     = $values['last'];

  $extra['param']['label']['tblClients.code']           = $values['code']     ? $values['code']                : WEBPAGE::$gt['all'];
  $extra['param']['label']['tblBusinessTypes.activity']	= $values['activity'] ? $activity[$values['activity']] : WEBPAGE::$gt['all'];
  $extra['param']['label']['tblBusiness.type_id']       = $values['type_id']  ? $type_id[$values['type_id']]   : WEBPAGE::$gt['all'];
  $extra['param']['label']['tblClients.first']          = $values['first']    ? $values['first']               : WEBPAGE::$gt['all'];
  $extra['param']['label']['tblClients.last']           = $values['last']     ? $values['last']                : WEBPAGE::$gt['all'];

  $extra['param']['ref'] = WEBPAGE::$scr_name;
  $extra['param']['ts']  = microtime(true);

  $fields[]                = "distinct b.name";
  $extra['name']['header'] = 'tblBusiness.name';
  $extra['name']['pack']   = '%s';
  $extra['name']['align']  = 'left';

  $fields[]                = "bt.type";
  $extra['type']['header'] = 'tblBusiness.type_id';
  $extra['type']['pack']   = '%s';
  $extra['type']['align']  = 'left';

  $fields[]                = 'b.id as view';
  $extra['view']['header'] = 'view';
  $extra['view']['pack']   = sprintf("<a href='%s?scr_name=BS.SCR.viewBusiness&ref=BS.SCR.browseBusinesses&id=%s'>%s</a>", $PHP_SELF,'%s',WEBPAGE::$gt['view']);
  $extra['view']['align']  = 'center';

  $fields[]                = 'b.id as edit';
  $extra['edit']['header'] = 'edit';
  $extra['edit']['pack']   = sprintf("<a href='%s?scr_name=BS.SCR.addBusiness&ref=BS.SCR.editBusiness&id=%s'>%s</a>", $PHP_SELF,'%s',WEBPAGE::$gt['edit']);
  $extra['edit']['align']  = 'center';

  if ( $values['activity'] ) { $operator_activity = '='; } else { $operator_activity = 'like'; $values['activity'] = '%%'; }
  if ( $values['type_id'] )  { $operator_type_id  = '='; } else { $operator_type_id  = 'like'; $values['type_id']  = '%%'; }
  if ( $values['code'] )     { $operator_code =     '='; } else { $operator_code     = 'like'; $values['code']     = '%%'; }

  $params = sprintf("
              c.code      %s    '%s'     and
              c.first      like '%%%s%%' and
              c.last       like '%%%s%%' and
              b.type_id    %s   '%s'     and
              bt.activity  %s   '%s'     and
              b.type_id     =    bt.id   and
              find_in_set(c.id, b.client_list)", $operator_code, $values['code'],
                                                 $values['first'],
                                                 $values['last'],
  					         $operator_type_id, $values['type_id'],
  					         $operator_activity, $values['activity']);

  $tables = 'tblBusiness b, tblBusinessTypes bt, tblClients c';

  $mrow = SQL::select($tables, $fields, $params);
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