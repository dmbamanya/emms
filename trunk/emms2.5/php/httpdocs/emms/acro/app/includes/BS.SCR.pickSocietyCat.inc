<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once 'class/society.php';

$btn  = new TTFButton(WEBPAGE::$theme);

$form = new HTML_QuickForm('pickCatForm','POST');

$form->addElement('static', 'submit',   '', $btn->imgsubmit('pickCatForm',WEBPAGE::$gt['submit']));
$form->addElement('static', 'cancel',   '', $btn->imglink($PHP_SELF,WEBPAGE::$gt['cancel']));
$form->addElement('hidden', 'scr_name',     $_REQUEST['scr_name']);

foreach (SOCIETY::categories() as $key=>$val)
{
  $cat_radio[] = $form->createElement('radio',null,null,WEBPAGE::$gt[sprintf('tblSocieties.category.%s',$key)],$key, 'class=radio');
}
$form->addGroup($cat_radio, 'cat', WEBPAGE::$gt['tblSocieties.category'],' ');
$form->addRule('cat',	WEBPAGE::$gt['required'],'required');

$form->validate() ? WEBPAGE::redirect(sprintf('index.php?scr_name=BS.SCR.addSociety&cat=%s',$form->_submitValues[cat])) : '';   

$tpl->setVariable('pickOptionToContinue', WEBPAGE::$gt['pickOptionToContinue']);

$renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
$renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
$renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
$form->accept($renderer);
    
?>