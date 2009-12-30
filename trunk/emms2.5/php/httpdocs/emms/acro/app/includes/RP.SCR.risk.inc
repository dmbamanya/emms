<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once 'Date.php';
require_once 'class/coolbutton.php';

$mode = array
        (
          'RP.SCR.riskByBranch'  => WEBPAGE::$gt['RP.SCR.riskByBranch'],
          'RP.SCR.riskByProgram' => WEBPAGE::$gt['RP.SCR.riskByProgram'],
          'RP.SCR.riskByAdvisor' => WEBPAGE::$gt['RP.SCR.riskByAdvisor']
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
$form->addElement('text',   'dateTo',       WEBPAGE::$gt['date'],          'class=large');
/*
 * add standard rules
 */
$form->addRule('mode',      WEBPAGE::$gt['selectOption'], 'required');
$form->addRule('dateTo',    'yyyy-mm-dd',                 'required');
$form->addRule('dateTo',    'yyyy-mm-dd',                 'regex','(^([0-9]{4}-[0-9]{2}-[0-9]{2})$)');
/*
 * register and add user-defined rules
 */
$form->registerRule('checkdateTo',   'callback', 'checkdateTo');
$form->addRule('dateTo',   WEBPAGE::$gt['nonValidEntry'], 'checkdateTo',   true);

if ($form->validate())
{
  $values = $form->_submitValues;
  WEBPAGE::redirect(sprintf('index.popup.php?scr_name=%s&date=%s',$values['mode'],$values['dateTo']));
  exit;
} else {
  echo '<script>window.close();</script>';
}

$form->setDefaults(array('dateTo'=>date('Y-m-d')));
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

?>
