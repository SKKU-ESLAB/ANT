function CodeEditorView(initialCode) {
  this.mRootDom = document.createElement('div');
  this.mRootDom.setAttribute('id', 'code_editor');
  this.mRootDom.setAttribute(
    'style',
    'width: 100%; height: 100%; border: 1px solid grey'
  );

  require.config({paths: {vs: './vs'}});
  require(['vs/editor/editor.main'], function () {
    var editor = monaco.editor.create(document.getElementById('code_editor'), {
      value: [initialCode].join('\n'),
      language: 'javascript'
    });
  });
}

CodeEditorView.prototype.getDom = function () {
  return this.mRootDom;
};
