<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';

$form = new HTML_QuickForm('searchForm','POST');

$form->addElement('static', 'submit', '', WEBPAGE::$btn->imgsubmit('searchForm',WEBPAGE::$gt['search']));
$form->addElement('static', 'reset',  '', WEBPAGE::$btn->imgreset('searchForm',WEBPAGE::$gt['reset']));
$form->addElement('static', 'cancel', '', WEBPAGE::$btn->imglink($PHP_SELF,WEBPAGE::$gt['cancel']));
$form->addElement('hidden', 'scr_name',   $_REQUEST['scr_name']);
$form->addElement('hidden', 'ref',        $_REQUEST['ref']);

$form->addElement('text',   'username',   WEBPAGE::$gt['tblUsers.username'],   'class=large');
$form->addElement('text',   'sponsor',    WEBPAGE::$gt['tblSponsors.sponsor'], 'class=large');
$form->addElement('text',   'email',      WEBPAGE::$gt['tblUsers.email'],      'class=large');

if ($form->validate())
{
  $values = $form->_submitValues;

  $extra['param']['value']['username'] = $values['username'];
  $extra['param']['value']['sponsor']  = $values['sponsor'];
  $extra['param']['value']['email']    = $values['email'];

  $extra['param']['label']['tblUsers.username']   = $values['username'] ? $values['username'] : WEBPAGE::$gt['all'];
  $extra['param']['label']['tblSponsors.sponsor'] = $values['sponsor']  ? $values['sponsor']  : WEBPAGE::$gt['all'];
  $extra['param']['label']['tblUsers.email']      = $values['email']    ? $values['email']    : WEBPAGE::$gt['all'];

  $extra['username']['header']     = 'tblUsers.username';
  $extra['username']['pack']       = '%s';
  $extra['username']['align']      = 'left';

  $extra['sponsor']['header']      = 'tblSponsors.sponsor';
  $extra['sponsor']['pack']        = '%s';
  $extra['sponsor']['align']       = 'left';

  $extra['active']['header']       = 'tblUsers.active';
  $extra['active']['pack']         = 'tblUsers.active.%s';
  $extra['active']['align']        = 'right';

  $extra['donations']['header']    = 'donations';
  $extra['donations']['pack']      = sprintf("<a href='javascript:openWin(\"index.popup.php?scr_name=SP.SCR.donationHistoryBySponsor&id=%s\",\"donationHistoryBySponsor\",\"menubar=no,scrollbars=yes,resizable=yes,width=700,height=320\")'>%s</a>",'%s',WEBPAGE::$gt['view']);
  $extra['donations']['align']     = 'center';

  $extra['add_donation']['header'] = 'addDonation';
  $extra['add_donation']['pack']   = sprintf("<a href='%s?scr_name=SP.SCR.addDonation&ref=SP.SCR.addDonation&sponsor_id=%s'>%s</a>", $PHP_SELF,'%s',WEBPAGE::$gt['addDonation']);
  $extra['add_donation']['align']  = 'center';

  $extra['view']['header']         = 'view';
  $extra['view']['pack']           = sprintf("<a href='%s?scr_name=SP.SCR.viewSponsor&ref=SP.SCR.browseSponsors&id=%s'>%s</a>", $PHP_SELF,'%s',WEBPAGE::$gt['view']);
  $extra['view']['align']          = 'center';

  $extra['edit']['header']         = 'edit';
  $extra['edit']['pack']           = sprintf("<a href='%s?scr_name=SP.SCR.addSponsor&ref=SP.SCR.editSponsor&id=%s'>%s</a>", $PHP_SELF,'%s',WEBPAGE::$gt['edit']);
  $extra['edit']['align']          = 'center';

  $extra['fsummary']['header']     = 'SP.SCR.financialSummary';
  $extra['fsummary']['pack']       = sprintf("<a href='%s?scr_name=SP.SCR.financialSummary&ref=SP.SCR.browseSponsor&id=%s'>%s</a>", $PHP_SELF,'%s',WEBPAGE::$gt['SP.SCR.financialSummary']);
  $extra['fsummary']['align']      = 'center';

  $extra['csummary']['header']     = 'SP.SCR.clientSummary';
  $extra['csummary']['pack']       = sprintf("<a href='%s?scr_name=SP.SCR.clientSummary&ref=SP.SCR.browseSponsor&id=%s'>%s</a>", $PHP_SELF,'%s',WEBPAGE::$gt['SP.SCR.clientSummary']);
  $extra['fsummary']['align']      = 'center';

  $extra['param']['ref'] = WEBPAGE::$scr_name;
  $extra['param']['ts']  = microtime(true);

  $mrow = WEBPAGE::$dbh->getAll(sprintf("
          select
            s.username,
            s.sponsor,
            s.active,
            s.id donations,
            s.id add_donation,
            s.id view,
            s.id edit,
            s.id fsummary,
            s.id csummary
          from
            tblSponsors s
          where
            s.username like '%%%s%%' and
            s.sponsor like '%%%s%%' and
            s.email like '%%%s%%'",$values['username'],$values['sponsor'],$values['email']));

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