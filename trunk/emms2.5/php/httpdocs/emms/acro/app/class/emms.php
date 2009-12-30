<?php
error_reporting(E_ERROR);
abstract class EMMS
{
  const _MOD_SPONSORSHIP_QUERY_CACHE	= "../../../../%s/%s/tmp/sponsorship.query.cache.%s";
  const _MOD_SPONSORSHIP_FN_CACHE	= "../../../../%s/%s/tmp/sponsorship.fn.%s";
  const _MOD_SPONSORSHIP_MENU	        = "../../../../%s/%s/sponsorship.menu.xml";


  const _CONF_INI                       = "../../../../%s/%s/conf.ini";
  const _APP_CONF_CACHE                 = "../../../../%s/%s/tmp/conf.data";
  const _APP_LOCALIZED_TEXTS            = "../l10n/strings.%s";
  const _APP_LOCALIZED_TEXTS_CACHE      = "../../../../%s/%s/tmp/strings.%s";
  const _APP_MENU		        = "../../../../%s/%s/menu.xml";
  const _APP_MENU_CACHE		        = "../../../../%s/%s/tmp/menu.data";
  const _APP_FN_CACHE		        = "../../../../%s/%s/tmp/fn.%s";
  const _APP_CUSTOM_TPL                 = "../../../../%s/%s/custom/templates";
  const _APP_CUSTOM_INC                 = "../../../../%s/%s/custom/includes";
  const _APP_RUN_MODE_FILE		= "%s/run.mode.data";
  const _APP_MODE_LOG_FILE		= "%s/run.mode.log.data";
  const _APP_QUERY_CACHE		= "../../../../%s/%s/tmp/query.cache.%s";

  const _DB_CONN_STR                    = "mysql://%s:%s@%s/%s";

  const _REQUIRED_FIELD 		= "{label}<span class=required size='1'>*</span>";
  const _FIELD_ERROR    		= "<span class=error>{error}</span><br />{html}";
  const _THEMES_PATH			= "./themes/";

  const _TREEMENU_IMG_PATH              = "./themes/%s/menu/";
  const _TREEMENU_ICON_FOLDER           = "folder.gif";
  const _TREEMENU_ICON_XFOLDER          = "folder-expanded.gif";
  const _TREEMENU_ICON_FILE             = "file.gif";
  const _TREEMENU_CSS_CAPTION           = "treeCaption";
  const _TREEMENU_CSS_BRANCH            = "treeBranch";
  const _TREEMENU_CSS_NODE              = "treeNode";

  const _IMG_USER			= "img/users/%s.jpg";
  const _IMG_CLIENT			= "img/clients/%s.jpg";

  const _PAGER_MODE    			= 'Jumping';//'Sliding';//Jumping
  const _PAGER_PERPAGE    		= 10;
  const _PAGER_DELTA    		= 10;
  const _PAGER_EXPANDED			= false;
  const _PAGER_LINKCLASS		= 'pager';
  const _PAGER_ALTPREV			= 'previous';
  const _PAGER_ALTNEXT			= 'next';
  const _PAGER_ALTPAGE			= 'gotopage';
  const _PAGER_IMG			= "<img src='%s%s/icons/%s' alt='%s'>";
  const _PAGER_PREVIMG			= 'previous.png';
  const _PAGER_NEXTIMG			= 'next.png';
  const _PAGER_FIRSTIMG			= 'first.png';
  const _PAGER_LASTIMG			= 'last.png';
  const _PAGER_SEPARATOR		= "&nbsp;&nbsp;&nbsp;&nbsp;";
  const _PAGER_FIRSTPAGETEXT	        = "firstPage";
  const _PAGER_LASTPAGETEXT		= "lastPage";
  const _PAGER_CURPAGELINKCLASS	        = 'pager';
  const _PAGER_EXTRA_SCR_NAME           = 'BS.SCR.pager';

  const _HOTLINK			= "<a class=record href='index.php?scr_name=%s&%s=%s'>%s</a>";
  const _HOTLINK_POPUP			= "<a title='%s' class=record href=\"javascript:openWin('index.popup.php?scr_name=%s&%s=%s','%s','menubar=%s,scrollbars=%s,resizable=%s,width=%s,height=%s')\">%s&nbsp;^</a>";
//  const _HOTICON			= "<a href='index.php?scr_name=%s&%s=%s'><img alt='%s' src='./themes/%s/icons/%s.png'></a>";

  const _RUN_MODE_OUTDATED              = 'outdated';
  const _RUN_MODE_NORMAL                = 'normal';
  const _RUN_MODE_MAINTENANCE           = 'maintenance';
  const _RUN_MODE_CRASHED               = 'crashed';
  const _RUN_MODE_DEBUG                 = 'debug';

  static $conf;
  static $gt;
  static $menu;
  static $fn;
  static $paths;
  static $url;
  static $runMode;
  static $runDate;
  static $scr_name;
  static $referrer;
  static $tabmenu;
  static $navtree;
  static $lang;
  static $screenWidth;
  static $theme;
  static $themePath;
  static $userID;
  static $userAccessCode;
  static $userName;
  static $userZone;
  static $zoneName;
  static $queryCache;
  static $auth_options;
  static $dbh;
  static $btn;

  abstract static function START();
  abstract static function LOAD_SESSION();
  abstract static function load_fn();
  abstract static function redirect($url);

  function EMMS()
  {
    require_once 'DB.php';
    require_once 'class/TTFButton.php';

    /*
     * parse app paths
     */
    self::$paths = array_reverse(explode('/',parse_url($_SERVER['PHP_SELF'],PHP_URL_PATH)));

    /*
     * load configuration params
     */
    self::load_conf();

    /*
     * set db connection
     */
    self::$auth_options['dsn'] = sprintf(self::_DB_CONN_STR,self::$conf['db']['usr'],self::$conf['db']['pwd'],self::$conf['db']['host'],self::$conf['db']['name']);
    self::$dbh = DB::connect(self::$auth_options['dsn']);
    self::$dbh->setFetchMode(DB_FETCHMODE_ASSOC);

    /*
     * set app page to launch
     */
    self::$scr_name = isset($_POST['scr_name']) ? $_POST['scr_name'] : self::$conf['app']['default_homepage'];
    self::$scr_name = isset($_GET['scr_name'])  ? $_GET['scr_name']  : self::$scr_name;

    /*
     * set app referrer page
     */
    self::$referrer = isset($_REQUEST['ref']) ? $_REQUEST['ref'] : '';

    /*
     * set app client width - within the browser
     */
    if (isset($_POST['screenWidth']))
    {
      self::$screenWidth = 0.95*$_POST['screenWidth'];
    }

    /*
     * set app theme
     */
    self::$theme = self::$conf['app']['default_theme'];
    self::$themePath = sprintf("%s%s/",self::_THEMES_PATH,self::$theme) ;
    self::$btn = new TTFButton(self::$theme);

    /*
     * set run mode
     */
    $data = current(self::$dbh->getAssoc("select id,mode,date from tblDataLog order by id desc limit 1"));
    self::$runDate = $data['date'];
    self::$runMode = $data['mode'];
    if (self::$runDate != date('Y-m-d'))
    {
      if (self::$runMode != self::_RUN_MODE_NORMAL)  { self::$runMode = self::_RUN_MODE_CRASHED;  }
      if (self::$runMode != self::_RUN_MODE_CRASHED) { self::$runMode = self::_RUN_MODE_OUTDATED; }
    }
    self::$runMode = self::$conf['app']['force_run_mode'] ? self::$conf['app']['force_run_mode'] : self::$runMode;

  }

  /*
   * returns data array from cache file
   */
  static function getCacheData ($cache)
  {
    if (file_exists($cache))
    {
      $file = fopen($cache, 'r');
      if ($file)
      {
        $data = fread($file,filesize($cache));
        fclose($file);
      }
    return unserialize(gzuncompress($data));
    }
  }

  /*
   * writes cache file from data array
   */
  static function makecachefile ($cache_data,$cache_file)
  {
    if (is_array($cache_data))
    {
      $data = gzcompress(serialize($cache_data),9);
      $file = fopen($cache_file, 'w');
      if ($file)
      {
        fwrite($file, $data);
        fclose($file);
      }
    }
  }

  /*
   * loads configurations params
   */
  static function load_conf()
  {
    if (!(file_exists($cache = sprintf(self::_APP_CONF_CACHE,self::$paths['3'],self::$paths['2']))))
    {
      /*
       * load conf from ini file and write to cache
       */
      self::$conf = parse_ini_file(sprintf(self::_CONF_INI,self::$paths['3'],self::$paths['2']),true);
      self::makecachefile(self::$conf,$cache);
    } else {
    /*
     * load conf from cache
     */
      self::$conf = self::getCacheData($cache);
    }
  }

  /*
   * loads localized data
   */
  static function load_gt()
  { 
    if (!(file_exists($cache = sprintf(self::_APP_LOCALIZED_TEXTS_CACHE,self::$paths['3'],self::$paths['2'],WEBPAGE::$lang))))
    { 
      /*
       * load localized texts from l10n file and write to cache
       */
      self::$gt = parse_ini_file(sprintf(self::_APP_LOCALIZED_TEXTS,WEBPAGE::$lang),true);
      /*
       * support localization of database fields with ENUM type.
       */
      foreach(self::$gt as $key => $val)
      { 
        if(($pos = strpos($key,'.optList'))&&( $pos + 8 == strlen($key)))
        {
          $inx = explode('.',$key);
          $optList = explode(',',$val);
          $optNames = explode(',',self::$gt[sprintf('%s.%s.optNames',$inx[0],$inx[1])]);
          foreach($optList as $i => $opt)
          {
            self::$gt[sprintf('%s.%s.%s',$inx[0],$inx[1],$opt)] = $optNames[$i];
          }
        }
      }
      self::makecachefile(self::$gt,$cache);
    } else {
    /*
     * load localized texts from cache
     */
      self::$gt = self::getCacheData($cache);
    } 
  }

  static function getPagerOptions($referrer,$data,$custom_PAGER_PERPAGE = 0)
  {
// 'firstPageText'			=> WEBPAGE::$gt[self::_PAGER_FIRSTPAGETEXT],
    return array(
                'mode'       		=> self::_PAGER_MODE,
                'perPage'    		=> $custom_PAGER_PERPAGE ? $custom_PAGER_PERPAGE : self::_PAGER_PERPAGE,
		'delta'      		=> self::_PAGER_DELTA,
		'linkClass'	 	=> self::_PAGER_LINKCLASS,
		'altPrev'	 	=> self::$gt[self::_PAGER_ALTPREV],
		'altNext'	 	=> self::$gt[self::_PAGER_ALTNEXT],
		'altFirst'	 	=> self::$gt[self::_PAGER_FIRSTPAGETEXT],
		'altLast'	 	=> self::$gt[self::_PAGER_LASTPAGETEXT],
		'altPage'	 	=> self::$gt[self::_PAGER_ALTPAGE],
		'spacesBeforeSeparator' => 0,
		'spacesAfterSeparator'  => 0,
		'separator'	 	=> self::_PAGER_SEPARATOR,
		'prevImg'		=> sprintf(self::_PAGER_IMG,self::_THEMES_PATH,self::$theme,self::_PAGER_PREVIMG,self::$gt[self::_PAGER_ALTPREV]),
		'nextImg'		=> sprintf(self::_PAGER_IMG,self::_THEMES_PATH,self::$theme,self::_PAGER_NEXTIMG,self::$gt[self::_PAGER_ALTNEXT]),
		'firstPageText'		=> sprintf(self::_PAGER_IMG,self::_THEMES_PATH,self::$theme,self::_PAGER_FIRSTIMG,self::$gt[self::_PAGER_FIRSTPAGETEXT]),
		'lastPageText'		=> sprintf(self::_PAGER_IMG,self::_THEMES_PATH,self::$theme,self::_PAGER_LASTIMG,self::$gt[self::_PAGER_LASTPAGETEXT]),
		'curPageLinkClassName'	=> self::_PAGER_CURPAGELINKCLASS,
		'firstPagePre'		=> '',
		'firstPagePost'		=> '',
		'lastPagePre'		=> '',
		'lastPagePost'		=> '',
		'itemData'		=> $data,
		'extraVars' 		=> array('scr_name'=>self::_PAGER_EXTRA_SCR_NAME, 'ref'=>$referrer['ref'], 'ts'=>$referrer['ts'])
	        );
  }

  static function printerlink()
  {
    return sprintf("<a href='javascript:print()'>%s</a>",self::$gt['printSheet']);
  }

  static function hotlink($scr_name,$par,$value,$text)
  {
    return sprintf(self::_HOTLINK,$scr_name,$par,$value,$text);
  }

  static function hoticon($scr_name,$par,$value,$text,$icon)
  {
    return sprintf(self::_HOTICON,$scr_name,$par,$value,$text,self::$theme,$icon);
  }

  static function hotlist($scr_name,$par,$list)
  {
    $hotlist = array();
    foreach ($list as $key => $value)
    {
      $hotlist[$key] = sprintf(self::_HOTLINK,$scr_name,$par,$key,$value);
    }
    return $hotlist;
  }

  static function appendParam2URL($par,$val)
  {
    return sprintf("%s?scr_name=%s&%s=%s",$PHP_SELF,self::$scr_name,$par,$val);
  }

  static function printmessage($ico,$msg)
  {
    $tpl = new HTML_Template_ITX('./templates');
    $tpl->loadTemplateFile('ST.SCR.message.tpl');

    $tpl->setCurrentBlock("message") ;
    $tpl->setVariable('ico',	sprintf('%s/icons/%s.png',self::$themePath, $ico));
    $tpl->setVariable('msg',	$msg);
    $tpl->parseCurrentBlock("message") ;

    return $tpl->get();
  }

  static function printchart($data,$head = array(),$pack = array())
  {
    $timestamp = microtime(true);
    array_unshift($data,$head);
    self::makecachefile($data,self::$queryCache.'.'.$timestamp);
    array_shift($data);

    $tpl = new HTML_Template_ITX('./templates');
    $tpl->loadTemplateFile('ST.chart.tpl');

    foreach($head as $key=>$val)
    {
      $tpl->setCurrentBlock("header") ;
      $tpl->setVariable('column_name', $val);
      $tpl->parseCurrentBlock("header") ;
    }

    foreach($data as $key=>$row)
    {
      foreach($row as $col => $val)
      {
        $tpl->setCurrentBlock("row") ;
        $tpl->setVariable('align', 'right');
        $tpl->setVariable('item', $pack[$col] ? self::$gt[sprintf($pack[$col],$val)] : $val);
        $tpl->parseCurrentBlock("row") ;
      }
      $tpl->setCurrentBlock("results") ;
      $tpl->parseCurrentBlock("results") ;
    }
    $tpl->setCurrentBlock("chart") ;
    $tpl->setVariable('xls_download',self::$gt['RP.SCR.ChartCacheToXLS']);
    $tpl->setVariable('timestamp',$timestamp);
    $tpl->parseCurrentBlock("chart") ;
    return $tpl->get();
  }

  static function printchart_ii($mdata,$head,$totals,$styles,$flags)
  {
    /*
    example for each param
    $mdata = array($data_0,$data_1);
      with:
       $data_0[row_0][col_0]='val_0_00';
       $data_0[row_0][col_1]='val_0_01';
       $data_1[row_0][col_0]='val_1_00';
       $data_1[row_0][col_1]='val_1_01';
    $head = array('col_0'=>'col_0_name', 'col_1'=>'col_1_name');
    $totals = array('totals'=>true,'subtotals'=>true,'cols'=>array('col_1'));
         // only calculate totals for 'col_1';
         // also show subtotals for each data set
         // and grand total row
    $styles = array(  'header'    => array('cell'=>'header_cell_class','row'=>''),
                      'subtotals' => array('cell'=>'subtotals_cell_class','row'=>'subtotals_row_class'),
                      'totals'    => array('cell'=>'totals_cell_class','row'=>'totals_row_class'),
                      'data_0'     => array('cell'=>'data_0_cells_class','row'=>'data_0_row_class'),
                      'data_1'     => array('cell'=>'data_1_cells_class','row'=>'data_1_row_class'));
    $flags = array('cache','legend');
    $flags['cache'] = true; // Save results in hd cache - makes for excel download.
    $flags['legend'] = true; // Prints legend.
    */

    if (!(is_array($mdata))) return self::$gt['noData'];
    foreach ($mdata as $inx => $data)
    {
      if (!count($data)) unset($mdata[$inx]);
    }
    if (!(count($mdata))) return self::$gt['noData'];
    /*
     * if style info is empty, then create default styles
     */
    if (!(count($styles)))
    {
      $styles['header']['cell']    = 'header';
      $styles['header']['row']     = '';
      $styles['subtotals']['cell'] = 'subtotals';
      $styles['subtotals']['row']  = 'subtotalsOff';
      $styles['totals']['cell']    = 'totals';
      $styles['totals']['row']     = 'totalsOff';
      foreach ($mdata as $inx=>$data)
      {
        if(count($data))
        {
          $row_style  = ($row_style  == 'rowOff_ii') ? 'rowOff_ii_alt' : 'rowOff_ii';
          $styles[$inx]['cell'] = 'activeChart';
          $styles[$inx]['row']  = $row_style;
        }
      }
    }
    /*
     * make sure we create a cache file to open in excel upon request
     */
    $c = 0;
    foreach($mdata as $inx=>$data)
    {
      foreach($data as $k=>$v)
      {
        $cdata[$c] = $v;
        $c++;
        if (is_array($totals['cols']))
        {
          foreach($v as $col=>$val)
          {
            if (in_array($col,$totals['cols']))
            {
              $subtotals[$inx][$col] += $val;
              $ctotals[$col]         += $val;
              } else {
              $subtotals[$inx][$col]  = '-';
              $ctotals[$col]          = '-';
            }
          }
        }
      }
    }

    if (is_array($ctotals)) $cdata[$c] = $ctotals;

    array_unshift($cdata,$head);
    if ($flags['cache'])
    {
      $timestamp = microtime(true);
      self::makecachefile($cdata,self::$queryCache.'.'.$timestamp);
    }
    array_shift($cdata);

    /*
     * print html chart
     */
    $tpl = new HTML_Template_ITX('./templates');
    $tpl->loadTemplateFile('ST.chart_ii.tpl');

    /*
     * print header row
     */
    $tpl->setCurrentBlock("header") ;
    $tpl->setVariable('column_name', '#');
    $tpl->setVariable('header_class', $styles['header']['cell']);
    $tpl->parseCurrentBlock("header") ;
    foreach($head as $key=>$val)
    {
      $tpl->setCurrentBlock("header") ;
      $tpl->setVariable('column_name', $val);
      $tpl->setVariable('header_class', $styles['header']['cell']);
      $tpl->parseCurrentBlock("header") ;
    }

    /*
     * print each data set, row by row
     * including subtotal for each set
     */
    $c = 1;
    foreach($mdata as $inx=>$data)
    {
      $row_style = $styles[$inx]['row'];
      //print regular rows
      foreach($data as $key=>$row)
      {
        $tpl->setCurrentBlock("row") ;
        $tpl->setVariable('cell_class', $styles[$inx]['cell']);
        $tpl->setVariable('item', sprintf('%s.&nbsp;',$c++));
        $tpl->parseCurrentBlock("row") ;
        foreach($row as $col => $val)
        {
          $tpl->setCurrentBlock("row") ;
          $tpl->setVariable('cell_class', $styles[$inx]['cell']);
          $tpl->setVariable('item', $val);
          $tpl->parseCurrentBlock("row") ;
        }
        $tpl->setCurrentBlock("results") ;
        $tpl->setVariable('row_class', $row_style);
        $tpl->parseCurrentBlock("results") ;
      }
      /*
       * print subtotals
       */
      if (($totals['subtotals'])&&(is_array($subtotals[$inx])))
      {
        $tpl->setCurrentBlock("row") ;
        $tpl->setVariable('cell_class', $styles['subtotals']['cell']);
        $tpl->setVariable('item', self::$gt['subtotal']);
        $tpl->parseCurrentBlock("row") ;
        foreach($subtotals[$inx] as $col => $val)
        {
          $tpl->setCurrentBlock("row") ;
          $tpl->setVariable('cell_class', $styles['subtotals']['cell']);
          $tpl->setVariable('item', is_numeric($val) ? round($val,2) : $val);
          $tpl->parseCurrentBlock("row") ;
        }
        $tpl->setCurrentBlock("results") ;
        $tpl->setVariable('row_class', $styles['subtotals']['row']);
        $tpl->parseCurrentBlock("results") ;
      }
    }
    /*
     * print totals row
     */
    if (($totals['totals'])&&(is_array($totals['cols'])))
    {
      $tpl->setCurrentBlock("row") ;
      $tpl->setVariable('cell_class', $styles['totals']['cell']);
      $tpl->setVariable('item', self::$gt['total']);
      $tpl->parseCurrentBlock("row") ;
      foreach($ctotals as $col => $val)
      {
        $tpl->setCurrentBlock("row") ;
        $tpl->setVariable('cell_class', $styles['totals']['cell']);
        $tpl->setVariable('item', is_numeric($val) ? round($val,2) : $val);
        $tpl->parseCurrentBlock("row") ;
      }
      $tpl->setCurrentBlock("results") ;
      $tpl->setVariable('row_class', $styles['totals']['row']);
      $tpl->parseCurrentBlock("results") ;
    }
    /*
     * print legend
     */
    if ($flags['legend'])
    {
      foreach($mdata as $inx=>$val)
      {
        if ($styles[$inx]['row'])
        {
          $tpl->setCurrentBlock("legend") ;
          $tpl->setVariable('style', $styles[$inx]['row']);
          $tpl->setVariable('legend_text', self::$gt[$inx] ? self::$gt[$inx] : $inx);
          $tpl->parseCurrentBlock("legend") ;
        }
      }
    }
    /*
     * finalize and print excel link
     */
    $tpl->setCurrentBlock("chart") ;
    if ($cache)
    {
      $tpl->setVariable('xls_download',self::$gt['RP.SCR.ChartCacheToXLS']);
      $tpl->setVariable('timestamp',$timestamp);
    }
    $tpl->parseCurrentBlock("chart") ;
    return $tpl->get();
  }

  static function verbose_date_format($date)
  {
    $fdate      = self::$gt['verbose_date_format'];
    $darray     = explode('-',$date);

    $date_unix  = mktime(0,0,0,$darray[1],$darray[2],$darray[0]);
    $month      = date('F',$date_unix);
    $weekday    = date('l',$date_unix);

    $fdate	= str_replace('%M', self::$gt[$month],     $fdate);
    $fdate	= str_replace('%W', self::$gt[$weekday],   $fdate);
    $fdate	= str_replace('%D', $darray[2], $fdate);

    return str_replace('%Y', $darray[0], $fdate);
  }

  static function validateFn($fn)
  {
    if ( intval(WEBPAGE::$userAccessCode) & intval(WEBPAGE::$fn['mask'][$fn]) ) return true;
    return false;
  }
}