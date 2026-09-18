#include "application/boxui/BoxUiSession.h"
#include "application/composition/DiagramServices.h"
#include "support/TestSupport.h"
#include <fstream>
using namespace xfmd;
void run() {
  std::ifstream f(XFMD_BOXUI_FIXTURE);
  std::string json((std::istreambuf_iterator<char>(f)), {});
  SourceSnapshot source{{91, 1}, "```boxui\nboxui 0.1\n" + json + "\n```", {}, false};
  auto parser = DiagramServices::interpreter();
  auto model = parser->parse(source);
  BoxUiSession session;
  session.expect(source.token);
  CHECK(!session.simulated());
  session.toggle(model);
  CHECK(session.simulated());
  auto state = session.freeze(source.token);
  auto first = state.blocks.at("activity-demo");
  CHECK(first.contextRevision == "1" &&
        std::get<double>(first.values.at("measurement").value) == 12.);
  auto intent = [&](const char* widget, const char* binding, const char* id) {
    auto s = session.freeze(source.token);
    BoxUiIntent i;
    i.key = {source.token, "activity-demo", s.epoch, s.bindingRevision, s.stateRevision, 0};
    i.widgetId = widget;
    i.bindingId = binding;
    i.eventId = id;
    i.commandId = id;
    i.contextRevision = s.blocks.at("activity-demo").contextRevision;
    return i;
  };
  auto pause = intent("pause", "suspend", "1");
  CHECK(session.dispatch(pause).status == "accepted");
  auto revision = session.freeze(source.token).stateRevision;
  CHECK(session.dispatch(pause).status == "accepted");
  CHECK(session.freeze(source.token).stateRevision == revision);
  auto conflict = pause;
  conflict.widgetId = "continue";
  CHECK(session.dispatch(conflict).status == "rejected");
  auto resume = intent("continue", "resume", "2");
  CHECK(session.dispatch(resume).status == "accepted");
  CHECK(session.dispatch(intent("pause", "suspend", "3")).status == "accepted");
  auto context = intent("context-input", "set-context", "4");
  context.value = std::string("C2");
  context.expectedValueRevision =
      session.freeze(source.token).blocks.at("activity-demo").values.at("context").revision;
  CHECK(session.dispatch(context).status == "accepted");
  auto rejected = session.dispatch(intent("continue", "resume", "5"));
  CHECK(rejected.status == "rejected" &&
        rejected.message.find("context changed") != std::string::npos);
  auto stale = pause;
  stale.commandId = "6";
  stale.eventId = "6";
  CHECK(session.dispatch(stale).status == "rejected");
  auto wrong = intent("context-input", "set-context", "7");
  wrong.value = std::string("C3");
  wrong.expectedValueRevision = "0";
  CHECK(session.dispatch(wrong).status == "rejected");
  auto frozen = session.freeze(source.token);
  session.expect({91, 2});
  CHECK(!session.simulated());
  CHECK(session.freeze({91, 2}).blocks.empty());
  CHECK(frozen.blocks.at("activity-demo").values.at("context").value ==
        BoxUiValue(std::string("C2")));
  // Bounded deduplication storage and input validation.
  session.expect(source.token);
  session.toggle(model);
  for (unsigned n = 0; n < 4096; ++n) {
    auto bad = intent("unknown", "resume", ("limit-" + std::to_string(n)).c_str());
    CHECK(session.dispatch(bad).status == "rejected");
  }
  CHECK(session.dispatch(intent("pause", "suspend", "overflow")).message.find("exhausted") !=
        std::string::npos);
  session.toggle(model);
  session.toggle(model);
  auto invalid = intent("context-input", "set-context", "utf8");
  invalid.value = std::string(1, char(255));
  invalid.expectedValueRevision =
      session.freeze(source.token).blocks.at("activity-demo").values.at("context").revision;
  CHECK(session.dispatch(invalid).status == "rejected");
  auto secondSource = source;
  auto secondText = source.text;
  secondText.replace(secondText.find("activity-demo"), 13, "second-demo");
  secondSource.text += "\n\n" + secondText;
  auto two = parser->parse(secondSource);
  session.toggle(model);
  session.toggle(two);
  auto change = intent("context-input", "set-context", "isolated");
  change.value = std::string("isolated-context");
  change.expectedValueRevision =
      session.freeze(source.token).blocks.at("activity-demo").values.at("context").revision;
  CHECK(session.dispatch(change).status == "accepted");
  CHECK(std::get<std::string>(
            session.freeze(source.token).blocks.at("second-demo").values.at("context").value) ==
        "C1");
  SyntheticActivity activity;
  CHECK(!activity.observe(1, 1, 999.));
  activity.execute("mark-stale", {});
  CHECK(activity.snapshot(*model->blocks[0].boxUi).values.at("measurement").validity == "stale");
  CHECK(!activity.observe(1, 1, 999.));
  activity.resetSource();
  CHECK(activity.snapshot(*model->blocks[0].boxUi).values.at("measurement").validity == "missing");
  CHECK(!activity.observe(1, 2, 999.));
  CHECK(activity.observe(2, 1, 7.));
}
TEST_MAIN(run)
