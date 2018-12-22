
<?php
    require 'dbh.inc.php';
?>


<!DOCTYPE html>
<html>
    <head>
            <title>Search Engine</title>

            <link rel="stylesheet" type="text/css" href="css/bootstrap.min.css">
            <link rel="stylesheet" type="text/css" href="font-awesome/css/font-awesome.min.css">
            <link rel="stylesheet" type="text/css" href="css/styles.css">

            <link rel="shortcut icon" href="img/logo.ico">
    </head>

    <body>
    
    <main role="main" class="container">
            
      <div class="d-flex align-items-center p-3 my-3 text-white-50 bg-purple rounded shadow-sm">
        
            <form class="form-inline" method="post" action="">
            
                <div class="form-group mx-sm-3 mb-2">
                  <input type="text" name="query" class="form-control" id="query" placeholder="Search Here">
                </div>
                <button type="submit" name="search-submit" class="btn btn-primary mb-2">Search</button>

            </form>
          
      </div>

      <div class="my-3 p-3 bg-white rounded shadow-sm">
        <h6 class="border-bottom border-gray pb-2 mb-0">Results</h6>
        <div class="media text-muted pt-3">
          <img data-src="holder.js/32x32?theme=thumb&bg=007bff&fg=007bff&size=1" alt="" class="mr-2 rounded">
          <p class="media-body pb-3 mb-0 small lh-125 border-bottom border-gray">
            <strong class="d-block text-gray-dark">@username</strong>
            Donec id elit non mi porta gravida at eget metus. Fusce dapibus, tellus ac cursus commodo, tortor mauris condimentum nibh, ut fermentum massa justo sit amet risus.
          </p>
        </div>
        <div class="media text-muted pt-3">
          <img data-src="holder.js/32x32?theme=thumb&bg=e83e8c&fg=e83e8c&size=1" alt="" class="mr-2 rounded">
          <p class="media-body pb-3 mb-0 small lh-125 border-bottom border-gray">
            <strong class="d-block text-gray-dark">@username</strong>
            Donec id elit non mi porta gravida at eget metus. Fusce dapibus, tellus ac cursus commodo, tortor mauris condimentum nibh, ut fermentum massa justo sit amet risus.
          </p>
        </div>
        <div class="media text-muted pt-3">
          <img data-src="holder.js/32x32?theme=thumb&bg=6f42c1&fg=6f42c1&size=1" alt="" class="mr-2 rounded">
          <p class="media-body pb-3 mb-0 small lh-125 border-bottom border-gray">
            <strong class="d-block text-gray-dark">@username</strong>
            Donec id elit non mi porta gravida at eget metus. Fusce dapibus, tellus ac cursus commodo, tortor mauris condimentum nibh, ut fermentum massa justo sit amet risus.
          </p>
        </div>
        <small class="d-block text-right mt-3">
          <a href="#">All updates</a>
        </small>
      </div>
    </main>
        
        
        
        <?php
        
            if(isset($_POST['search-submit']))
            {
                
                $query = $_POST['query'];
                
                exec("PHPtest.bin 1 ".$query, $output);
                
                echo '<h1>'.$output[0].'</h1>';
                
                $_POST = array();
            }
        
        ?>
        
        
        
        
        
        
        
           
    </body>
</html>