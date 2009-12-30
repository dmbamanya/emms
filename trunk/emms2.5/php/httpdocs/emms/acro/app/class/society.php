<?php
class SOCIETY extends WEBPAGE
{  	
  /* SOCIETY parameters */
  var $frostdata 	= array();
  var $data  		= array( 'creator_id' => 0);
  var $fields 		= array();

  function SOCIETY($id = '',$category = '')  //class constructor
  {
    if ($id)
    {
      $fields = sprintf("s.*, ls.parent_id, concat((lpad(z.id,3,'0')),'.',s.category,'.',(lpad(s.id,5,'0'))) code,concat(u.first,' ',u.middle,' ',u.last) as advisor,u.username as advisor_username,concat(uc.first,' ',uc.last) creator,date_format(s.creator_date,'%s') f_creator_date, concat(ue.first,' ',ue.last) editor, date_format(s.editor_date,'%s') f_editor_date, concat(pz.zone,' : ',p.program) zone", WEBPAGE::$gt['date_format_mysql'], WEBPAGE::$gt['date_format_mysql']);
      $tables = "tblSocieties s, tblUsers uc, tblUsers ue";
      $this->data = current(WEBPAGE::$dbh->getAll(sprintf('select %s from (%s) left join tblLinkSocieties ls on s.id = ls.child_id and ls.child_id != ls.parent_id left join tblUsers u on s.advisor_id = u.id left join tblZones z on z.id = s.zone_id left join tblZones pz on pz.id = z.parent_id left join tblPrograms p on z.program_id = p.id where s.id = %s and s.creator_id = uc.id and s.editor_id = ue.id', $fields, $tables, $id)));
      $this->frostdata = array('category');
    } else {
      $this->data['creator_date'] = date('Y-m-d');
      if ($category)
      {
        $this->data['category'] = $category;
	$this->frostdata = array('category'); 
      } else {
        $this->frostdata = array();
      }
    }
  }

  function getTemplateData($id,$xpress = false)
  {
    $soc = new SOCIETY($id);
    $soc->data['advisor'] = $xpress ? $soc->data['advisor'] : sprintf(WEBPAGE::_HOTLINK,'BS.SCR.viewUser','id',$soc->data['advisor_id'],$soc->data['advisor']);
    $zone = self::zones();
    $soc->data['zone'] = $zone[$soc->data['zone_id']];
    $fields = "ls.parent_id, s.name";
    $tables = "tblLinkSocieties AS ls, tblSocieties AS s";
    $param  = sprintf("ls.child_id = '%s' AND ls.child_id != ls.parent_id AND s.id = ls.parent_id", $id);
    $parent = current(SQL::select($tables, $fields, $param));
    if ( $parent )
    {
      $soc->data['parent_id'] = $parent['parent_id'];
      $soc->data['parent']    = $xpress ? $soc->data['parent'] : sprintf(WEBPAGE::_HOTLINK,'BS.SCR.viewSociety','id',$parent['parent_id'],$parent['name']);
    }
    $soc->getMembers();
    $data = $soc->data;
    if ($soc->data['zone_id']&& $soc->data['advisor_id'] )
    {
      $c = 0;
      $data['buttondata'][$c]['id']      = "BS.SCR.addSociety";
      $data['buttondata'][$c]['href']    = "index.php?scr_name=BS.SCR.addSociety&id=".$id;
      $data['buttondata'][$c]['alt']     = WEBPAGE::$gt['edit'];
      $data['buttondata'][$c]['onClick'] = "";
      $data['buttondata'][$c]['ico']     = "edit";
      switch ($soc->data['category'])
      {
      case 'BG':
        $c++;
        $data['buttondata'][$c]['id']         = "BS.SCR.addSociety";
        $data['buttondata'][$c]['href']       = "index.php?scr_name=BS.SCR.addSociety&parent_id=".$id;
        $data['buttondata'][$c]['alt']        = WEBPAGE::$gt['newGroup'];
        $data['buttondata'][$c]['onClick']    = "";
        $data['buttondata'][$c]['ico']        = "society_add";
        $c++;
        $data['buttondata'][$c]['id']         = "BS.SCR.deactivateBG";
        $data['buttondata'][$c]['href']       = sprintf("index.php?scr_name=BS.SCR.deactivateBG&id=%s&ico=wrn&msg=inf.deactivateBG",$id);
        $data['buttondata'][$c]['alt']        = WEBPAGE::$gt['deactivate'];
        $data['buttondata'][$c]['onClick']    = "";
        $data['buttondata'][$c]['ico']        = "society_rem";
        break;
       case 'B':
         $c++;
         $data['buttondata'][$c]['id']        = "BS.SCR.addClient";
         $data['buttondata'][$c]['href']      = "index.php?scr_name=BS.SCR.addClient&society_id=".$id;
         $data['buttondata'][$c]['alt']       = WEBPAGE::$gt['newClient'];
         $data['buttondata'][$c]['onClick']   = "";
         $data['buttondata'][$c]['ico']       = "client_add";
         $c++;
         $data['buttondata'][$c]['id']        = "BS.SCR.moveSociety";
         $data['buttondata'][$c]['href']      = sprintf("index.php?scr_name=BS.SCR.moveSociety&id=%s",$id);
         $data['buttondata'][$c]['alt']       = WEBPAGE::$gt['BS.SCR.moveSociety'];
         $data['buttondata'][$c]['onClick']   = "";
         $data['buttondata'][$c]['ico']       = "society_move";
         $c++;
         $data['buttondata'][$c]['id']        = "BS.SCR.deactivateBGMember";
         $data['buttondata'][$c]['href']      = sprintf("index.php?scr_name=BS.SCR.deactivateBGMember&parent_id=%s&child_id=%s&ico=wrn&msg=inf.deactivateGroup",$soc->data['parent_id'],$id);
         $data['buttondata'][$c]['alt']       = WEBPAGE::$gt['deactivate'];
         $data['buttondata'][$c]['onClick']   = "";
         $data['buttondata'][$c]['ico']       = "society_rem";
         break;
       case 'G':
         $c++;
         $data['buttondata'][$c]['id']        = "BS.SCR.addClient";
         $data['buttondata'][$c]['href']      = "index.php?scr_name=BS.SCR.addClient&society_id=".$id;
         $data['buttondata'][$c]['alt']       = WEBPAGE::$gt['newClient'];
         $data['buttondata'][$c]['onClick']   = "";
         $data['buttondata'][$c]['ico']       = "client_add";
         break;
       }
    }
    return $data;
  }

  function advisors() 
  {
    return SQL::getAssoc_order('tblUsers','id,concat(last,", ",first, " ",middle)',sprintf("( access_code & %s ) != 0", ROLES::ADVISOR),'last');
  }

  function zones() 
  {
    return SQL::getAssoc_order('tblZones z, tblPrograms p, tblZones pz','z.id,concat(pz.zone," : ",p.program)','z.parent_id !=0 and p.id = z.program_id and pz.id = z.parent_id','z.parent_id');
  }

  function categories()
  {
    $opt = explode(",", WEBPAGE::$gt['tblSocieties.category.optList']);
    $nam = explode(",", WEBPAGE::$gt['tblSocieties.category.optNames']);
    foreach ($opt as $key => $val)
    {
      $cat[$val] = $nam[$key];
    }
    return $cat;
  }
  
  function getMembers() //also load the confidential atribute
  {
    // Get all groups involved - type G societies
    $mrow = SQL::select('tblLinkSocieties ls, tblSocieties s','ls.*, s.name',sprintf("ls.parent_id = '%s' and ls.parent_id != ls.child_id and s.id = ls.child_id",$this->data['id']));
    
    for($i=0; $i<count($mrow); $i++)
    {
      $this->data['members'][$mrow[$i]['child_id']]['name']                        = $mrow[$i]['name'];
      $this->data['members'][$mrow[$i]['child_id']]['member']                      = array();
      $this->data['members'][$mrow[$i]['child_id']]['deactivatebutton']['id']      = "rem".$mrow[$i]['child_id'];
      $this->data['members'][$mrow[$i]['child_id']]['deactivatebutton']['alt']     = WEBPAGE::$gt['deactivate'];
      $this->data['members'][$mrow[$i]['child_id']]['deactivatebutton']['ico']     = "deactivate";
      $this->data['members'][$mrow[$i]['child_id']]['deactivatebutton']['href']    = sprintf('javascript:openWin("index.popup.php?scr_name=BS.SCR.deactivateBGMember&parent_id=%s&amp;child_id=%s&amp;rem=1","Remove","menubar=no,scrollbars=no,resizable=no,width=360,height=240")',$this->data['id'],$mrow[$i]['child_id']);
      $this->data['members'][$mrow[$i]['child_id']]['deactivatebutton']['onClick'] = "return true";
      $this->data['members'][$mrow[$i]['child_id']]['movebutton']['id']            = "move".$mrow[$i]['child_id'];
      $this->data['members'][$mrow[$i]['child_id']]['movebutton']['alt']           = WEBPAGE::$gt['BS.SCR.moveSociety'];
      $this->data['members'][$mrow[$i]['child_id']]['movebutton']['ico']           = "move";
      $this->data['members'][$mrow[$i]['child_id']]['movebutton']['href']          = sprintf('index.php?scr_name=BS.SCR.moveSociety&id=%s',$mrow[$i]['child_id']);
      $this->data['members'][$mrow[$i]['child_id']]['movebutton']['onClick']       = "return true";
    } // Get all clients

    $client = new CLIENT();
    
    $fields = "c.id, concat(c.first,' ',c.last) client, c.society_id, s.name, c.advisor_id";
    $tables = "tblClients c, tblLinkSocieties ls, tblSocieties s";
    $param  = sprintf("ls.parent_id = '%s' and ls.child_id = c.society_id and s.id = c.society_id", $this->data['id']);
    $order  = 'c.society_id';
    $mrow   = SQL::select_order($tables,$fields,$param,$order);
    $num = count($mrow);

    for ($i=0; $i<$num; $i++)
    {
      $row = $mrow[$i];
      $client->data['id'] = $row['id'];
      $client->data['advisor_id'] = $row['advisor_id'];
      $client->data['society_id'] = $row['society_id'];
      $client->data['name'] = $row['client'];
      $client->checkConfidentiality();
      $this->data['confidential'] = $this->data['confidential'] || $client->data['confidential'];
      if ( $this->data['president_id'] == $row['id'] ) { $row['president']= "P"; }
      if ( $this->data['treasurer_id'] == $row['id'] ) { $row['treasurer']= "T"; }
      if ( $this->data['secretary_id'] == $row['id'] ) { $row['secretary']= "S"; }
      $this->data['members'][$client->data['society_id']]['member'][$row['id']]['name']      = $client->data['name'];
      $this->data['members'][$client->data['society_id']]['member'][$row['id']]['president'] = $row['president'];
      $this->data['members'][$client->data['society_id']]['member'][$row['id']]['treasurer'] = $row['treasurer'];
      $this->data['members'][$client->data['society_id']]['member'][$row['id']]['secretary'] = $row['secretary'];
      $members[$row['id']] = $client->data['name'];
      $member_lst[] = $row['id'];
    }
    return $members;
  } 
  
  function nameAutoUpdate() 
  {
    $this->data['parent_id'] = $this->data['parent_id'] ? $this->data['parent_id'] : $this->data['id'];
    switch ($this->data['category'])
    {
    case 'G':
      return true;
    default:
      $mrow = WEBPAGE::$dbh->getAll(sprintf('select ls.child_id,s.name from tblLinkSocieties ls, tblSocieties s where ls.child_id != parent_id and s.id = ls.parent_id and ls.parent_id = %s',$this->data['parent_id']));
      $c = 1;
      foreach ($mrow as $key=>$val)
      {
        $group_name = sprintf(WEBPAGE::$conf['app']['group.name.pattern'],$val['name'],$c);
        WEBPAGE::$dbh->query(sprintf("update tblSocieties set name = '%s' where id = %s",$group_name,$val['child_id']));
        $c++;
      }
      break;
    }
  }
  
}
?>