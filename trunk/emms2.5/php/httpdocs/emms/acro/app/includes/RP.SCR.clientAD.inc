<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once 'Date.php';
require_once 'class/coolbutton.php';

$mode = array
        (
          'RP.SCR.clientADActivations'   => WEBPAGE::$gt['RP.SCR.clientADActivations'],
          'RP.SCR.clientADDeactivations' => WEBPAGE::$gt['RP.SCR.clientADDeactivations']
        );
$btn  = new TTFButton(WEBPAGE::$theme);
/*
 * create form instance
 */
$form = new HTML_QuickForm('LForm','POST','index.popup.php','','onSubmit="popupform(this)"');
/*
 * add form elements
 */
$form->addElement('image',  'submit',       sprintf("%s?txt=%s",$btn->webface,WEBPAGE::$gt['submit']));
$form->addElement('hidden', 'scr_name',     $_REQUEST['scr_name']);
$form->addElement('select', 'mode',         WEBPAGE::$gt['mode'],     $mode, 'size="3" class=large');
$form->addElement('text',   'dateFrom',     WEBPAGE::$gt['dateFrom'],        'class=large');
$form->addElement('text',   'dateTo',       WEBPAGE::$gt['dateTo'],          'class=large');
/*
 * add standard rules
 */
$form->addRule('mode',      WEBPAGE::$gt['selectOption'], 'required');
$form->addRule('dateFrom',  'yyyy-mm-dd',                 'required');
$form->addRule('dateFrom',  'yyyy-mm-dd',                 'regex','(^([0-9]{4}-[0-9]{2}-[0-9]{2})$)');
$form->addRule('dateTo',    'yyyy-mm-dd',                 'required');
$form->addRule('dateTo',    'yyyy-mm-dd',                 'regex','(^([0-9]{4}-[0-9]{2}-[0-9]{2})$)');
/*
 * register and add user-defined rules
 */
$form->registerRule('checkdateTo',   'callback', 'checkdateTo');
$form->registerRule('checkdateFrom', 'callback', 'checkdateFrom');
$form->addRule('dateTo',   WEBPAGE::$gt['nonValidEntry'], 'checkdateTo',   true);
$form->addRule('dateFrom', WEBPAGE::$gt['nonValidEntry'], 'checkdateFrom', true);

if ($form->validate())
{
  $values = $form->_submitValues;
  WEBPAGE::redirect(sprintf('index.popup.php?scr_name=%s&date_from=%s&date_to=%s',$values['mode'],$values['dateFrom'],$values['dateTo']));
  exit;
} else {
  echo '<script>window.close();</script>';
}

$form->setDefaults(array('dateFrom'=>date('Y-m-d'),'dateTo'=>date('Y-m-d')));
$renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
$renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
$renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
$form->accept($renderer);

/*
 * callback functions in user-defined rules
 */
function checkdateTo($dateTo)
{
  $dateTo_check = explode('-',$dateTo);
  if (!checkdate($dateTo_check[1],$dateTo_check[2],$dateTo_check[0])) { return false; } //verifies that $dateTo is a real date
  if (date('Y-m-d') < $dateTo) { return false; } else { return true; }
}

function checkdateFrom($dateFrom)
{
  $dateFrom_check = explode('-',$dateFrom);
  if (!checkdate($dateFrom_check[1],$dateFrom_check[2],$dateFrom_check[0])) { return false; } //verifies that $dateFrom is a real date
  if ($dateFrom > $_REQUEST['dateTo']) { return false; } else { return true; }
}


?>
