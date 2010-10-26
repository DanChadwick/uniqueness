// $Id$

Drupal.behaviors.uniqueness = function (context) {
  uniqueness = new Drupal.uniqueness(Drupal.settings.uniqueness['URL'], $('.uniqueness-dyn'));
  // Search off title.
  $('#edit-title').keyup(function() {
    input = this.value;
    if (input.length >= uniqueness.minCharacters) {
      uniqueness.search('title', input);
    }
    else if(input.length == 0 && !uniqueness.prependResults) {
      uniqueness.clear();
    }
  })
  .keyup();     // Call immediately upon page load
  // Search off tags.
  $('#edit-taxonomy-tags-1').blur(function() {
    input = this.value;
    // Some tags set.
    if (input.length > 0) {
      uniqueness.search('tags', input);
    }
  });
  
};

Drupal.uniqueness = function (uri, widget) {
  this.uri = uri;
  this.delay = 500;
  this.widget = widget;
  this.prependResults = Drupal.settings.uniqueness['prependResults'];
  this.minCharacters = Drupal.settings.uniqueness['minCharacters'];
  this.nid = Drupal.settings.uniqueness['nid'];
  this.type = Drupal.settings.uniqueness['type'];
  if (this.prependResults) {
    this.widget.append('<p class="uniqueness-dyn-notifier"></p>').append('<ul class="uniqueness-dyn-ul"></ul>');
  }
  else {
    this.widget.append('<ul class="uniqueness-dyn-ul"></ul>').append('<p class="uniqueness-dyn-notifier"></p>');
  }
  this.list = $('ul.uniqueness-dyn-ul');
  this.notifier = $('.uniqueness-dyn-notifier');
  this.fieldset = $('.uniqueness-fieldset');
  this.prompt = $('.uniqueness-prompt');
  if (this.prompt) {
    this.originalPrompt = this.prompt.html();
  }
  this.widgetCSS = {
    'background-image' : "url('" + Drupal.settings.basePath + "misc/throbber.gif" + "')",
    'background-position' : '100% -18px',
    'background-repeat' : 'no-repeat'
  }
  this.searchCache = {};
  this.listCache = {};
}

Drupal.uniqueness.prototype.update = function (data) {
  uniqueness.notifier.removeClass('uniqueness-dyn-searching').empty();
  uniqueness.widget.css('background-image', '');
  uniqueness = this;
  if (uniqueness.prependResults) {
    if (data == undefined && uniqueness.listCache != {}) {
      data = uniqueness.listCache;
    }
    var items = '';
    $.each(data, function(i, item) {
      // Only use what we haven't seen before.
      if (uniqueness.listCache[item.nid] == undefined) {
        items += '<li><a href="' + item.href + '" target="_blank">' + item.title + '</a></li>';
        // Store the new item.
        uniqueness.listCache[item.nid] = item;
      }
    });
    // Show list.
    this.list.prepend(items);
    if (items.length > 0) {
      uniqueness.fieldset.removeClass('collapsed');
    }
  }
  else { // Replace content. //@todo still use caching?
    if (data == undefined) {
      uniqueness.clear();
      if (uniqueness.prompt && $('#edit-title')[0].value.length > 0) {
          uniqueness.prompt.html('<span class="uniqueness-success">Success!</span> No duplicates found.');
      }
      return;
    }
    var items = '';
    $.each(data, function(i, item) {
      if (item.more) {
        uniqueness.notifier.append('... and others.');
      }
      else {
        items += '<li><a href="' + item.href + '" target="_blank">' + item.title + '</a></li>';
      }
    });
    this.list.html(items);
    uniqueness.fieldset.removeClass('collapsed');
    if (uniqueness.prompt) {
      uniqueness.prompt.html(uniqueness.originalPrompt);
    }
  }
}

Drupal.uniqueness.prototype.search = function (element, searchString) {
  uniqueness = this;

  // If this string has been searched for before we do nothing.
  if (uniqueness.prependResults && uniqueness.searchCache[searchString]) {
    return;
  }

  if (this.timer) {
    clearTimeout(this.timer);
  }
  this.timer = setTimeout(function () {
    // Inform user we're searching.
    if (uniqueness.notifier.hasClass('uniqueness-dyn-searching') == false) {
      uniqueness.notifier.addClass('uniqueness-dyn-searching').html('Searching ...');
      uniqueness.widget.css(uniqueness.widgetCSS);
    }
    var query = uniqueness.uri + '?';
    if (uniqueness.nid != undefined) {
      query += 'nid=' + uniqueness.nid + '&';
    }
    if (uniqueness.type != undefined) {
      query += 'type=' + uniqueness.type + '&';
    }
    $.getJSON(query + element + '=' + searchString, function (data) {
      if (data != undefined && data != 'false') {
        // Found results.
        uniqueness.update(data);
        // Save this string, it found results.
        uniqueness.searchCache[searchString] = searchString;
      }
      // Nothing new found so show existing results.
      else {
        uniqueness.update();
      }
    });
  }, uniqueness.delay);
}

Drupal.uniqueness.prototype.clear = function () {
  this.list.empty();
  this.notifier.empty();
}