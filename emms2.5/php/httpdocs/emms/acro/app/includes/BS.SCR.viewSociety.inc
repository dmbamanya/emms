<?php
require_once 'class/coolbutton.php';
require_once 'class/roles.php';
require_once 'class/persons.php';
require_once 'class/client.php';
require_once 'class/society.php';

extract(SOCIETY::getTemplateData($_REQUEST['id'],$xpress));

$btn  = new TTFButton(WEBPAGE::$theme);

$tpl->setVariable('society',        $name);
$tpl->setVariable('code_label',     WEBPAGE::$gt['tblSocieties.code']);
$tpl->setVariable('code',           $code);
$tpl->setVariable('category_label', WEBPAGE::$gt['tblSocieties.category']);
$tpl->setVariable('category',       WEBPAGE::$gt[sprintf('tblSocieties.category.%s',$category)]);
if ($parent)
{
  $tpl->setVariable('bde_label',    WEBPAGE::$gt['tblSocieties.category.BG']);
  $tpl->setVariable('bde',          $parent);
}
$tpl->setVariable('advisor_label',  WEBPAGE::$gt['tblSocieties.advisor_id']);
$tpl->setVariable('advisor',        $advisor);
$tpl->setVariable('zone_label',     WEBPAGE::$gt['tblSocieties.zone_id']);
$tpl->setVariable('zone',           $zone);
$tpl->setVariable('memo_label',     WEBPAGE::$gt['tblSocieties.memo']);
$tpl->setVariable('memo',           $memo);
$tpl->setVariable('creator',        sprintf(WEBPAGE::$gt['createdByOn'],$creator,$f_creator_date));
$tpl->setVariable('editor',         sprintf(WEBPAGE::$gt['editedByOn'],$editor,$f_editor_date));

/*
$tpl->setVariable('refreshForm',    sprintf("<form name=refreshForm method=get>
                                               <input type=hidden name=scr_name value='%s'>
                                               <input type=hidden name=id value='%s'>
                                               <input type=hidden name=ref value='%s'>
                                             </form>",WEBPAGE::$scr_name,$_REQUEST['id'],$_REQUEST['ref']));
*/

if (is_array($members))
{
  $tpl->setVariable('memberlist_label',	WEBPAGE::$gt['memberlist']);
  $tpl->setVariable('legend_president',	WEBPAGE::$gt['president']);
  $tpl->setVariable('legend_treasurer', WEBPAGE::$gt['treasurer']);
  $tpl->setVariable('legend_secretary', WEBPAGE::$gt['secretary']);

  foreach ($members as $key => $data)
  {
    $tpl->setCurrentBlock("memberlist");  
    foreach ($data['member'] as $i => $row)
    {
      $tpl->setCurrentBlock("memberdetails");
      $tpl->setVariable("member_name", $xpress ? $row['name'] : sprintf(WEBPAGE::_HOTLINK,'BS.SCR.viewClient','id',$i,$row['name'])) ;
      $tpl->setVariable("president", $row['president']) ;
      $tpl->setVariable("treasurer", $row['treasurer']) ;
      $tpl->setVariable("secretary", $row['secretary']) ;
      $tpl->parseCurrentBlock("memberdetails");
    }
    $tpl->setCurrentBlock("memberlist");
    if (!($parent))
    {
      $tpl->setVariable("group_name", $xpress ? $data['name'] : sprintf(WEBPAGE::_HOTLINK,'BS.SCR.viewSociety','id',$key,$data['name'])) ;
    }
    $tpl->parseCurrentBlock("memberlist");
  }
}

include './includes/ST.SCR.buttonMan.inc';

?>